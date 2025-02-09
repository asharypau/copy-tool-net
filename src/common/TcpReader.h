#ifndef TCP_READER_H
#define TCP_READER_H

#include <boost/asio.hpp>

#include "../utils/Logger.h"

class TcpReader
{
public:
    explicit TcpReader(boost::asio::ip::tcp::socket& socket);

    // Disallow copying and assignment.
    TcpReader(const TcpReader&) = delete;
    TcpReader& operator=(const TcpReader&) = delete;

    // Disallow movement and assignment.
    TcpReader(TcpReader&& other) = delete;
    TcpReader& operator=(TcpReader&&) = delete;

    void read(size_t size_in_bytes);

    template <class TCallback>
    void read_async(size_t size_in_bytes, TCallback&& callback)
    {
        boost::asio::async_read(
            _socket,
            _buffer,
            boost::asio::transfer_exactly(size_in_bytes),
            [this, callback = std::forward<TCallback>(callback)](const boost::system::error_code& error, size_t read_bytes)
            {
                try
                {
                    if (error)
                    {
                        handle_error(error);
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

private:
    void handle_error(const boost::system::error_code& error);

    boost::asio::ip::tcp::socket& _socket;
    boost::asio::streambuf _buffer;
};

#endif
