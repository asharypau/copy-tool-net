#ifndef SESSION_H
#define SESSION_H

#include <boost/asio.hpp>
#include <memory>

#include "../utils/Logger.h"
#include "FileHandler.h"

class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(boost::asio::ip::tcp::socket socket, size_t client_id);

    void run();

private:
    void get_headers();
    void get_file(std::shared_ptr<FileHandler> file_handler);

    void get(size_t size_in_bytes);
    void handle_error(const boost::system::error_code& error);

    template <class TData>
    TData get_from_buffer()
    {
        const TData* raw_data = boost::asio::buffer_cast<const TData*>(_buffer.data());
        _buffer.consume(sizeof(*raw_data));

        return *raw_data;
    }

    template <class TData>
    void extract(TData* data, size_t size_in_bytes)
    {
        if (_buffer.size() < size_in_bytes)
        {
            throw std::runtime_error("Buffer underflow");
        }

        const TData* raw_data = boost::asio::buffer_cast<const TData*>(_buffer.data());
        std::memcpy(data, raw_data, size_in_bytes);

        _buffer.consume(size_in_bytes);
    }

    template <class TCallback>
    void get_async(size_t size_in_bytes, TCallback&& callback)
    {
        std::shared_ptr<Session> self = shared_from_this();
        boost::asio::async_read(
            _socket,
            _buffer,
            boost::asio::transfer_exactly(size_in_bytes),
            [self, callback = std::forward<TCallback>(callback)](const boost::system::error_code& error, size_t read_bytes)
            {
                try
                {
                    if (error)
                    {
                        self->handle_error(error);
                    }
                    else
                    {
                        callback();
                    }
                }
                catch (const std::exception& ex)
                {
                    Logger::error("Error occurred: " + std::string(ex.what()));
                }
            });
    }

    static constexpr size_t HEADER_SIZE = sizeof(size_t);

    size_t _client_id;
    boost::asio::ip::tcp::socket _socket;
    boost::asio::streambuf _buffer;
};

#endif  // SESSION_H
