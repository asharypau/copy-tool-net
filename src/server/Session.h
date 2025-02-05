#ifndef SESSION_H
#define SESSION_H

#include <boost/asio.hpp>

#include "../utils/Logger.h"
#include "FileHandler.h"

class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(boost::asio::ip::tcp::socket socket, size_t client_id);

    void start();

private:
    void read_file_size();
    void read_file_name(std::shared_ptr<FileHandler> file);
    void read_batch(std::shared_ptr<FileHandler> file);
    void handle_error(const boost::system::error_code& error);

    template <class TData, class THandler>
    void read_async(TData* data, size_t size_in_bytes, THandler&& handle)
    {
        std::shared_ptr<Session> self = shared_from_this();
        boost::asio::async_read(
            _socket,
            boost::asio::buffer(data, size_in_bytes),
            [self, handle = std::forward<THandler>(handle)](const boost::system::error_code& error, size_t read_bytes)
            {
                try
                {
                    if (error)
                    {
                        self->handle_error(error);
                    }
                    else
                    {
                        handle();
                    }
                }
                catch (const std::exception& ex)
                {
                    Logger::error("Error occurred: " + std::string(ex.what()));
                }
            });
    }

    size_t _client_id;
    boost::asio::ip::tcp::socket _socket;
};

#endif  // SESSION_H
