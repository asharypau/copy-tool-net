#ifndef TCP_WRITER_H
#define TCP_WRITER_H

#include <boost/asio.hpp>

#include "../utils/Logger.h"

class TcpWriter
{
public:
    explicit TcpWriter(boost::asio::ip::tcp::socket& socket);

    template <class TData, class TCallback>
    void write(TData* data, size_t size_in_bytes, TCallback&& callback)
    {
        boost::asio::async_write(
            _socket,
            boost::asio::buffer(data, size_in_bytes),
            [this, callback = std::forward<TCallback>(callback)](const boost::system::error_code& error, size_t read_bytes) mutable
            {
                try
                {
                    callback();
                }
                catch (const std::exception& ex)
                {
                    Logger::error("Error occurred: " + std::string(ex.what()));
                }
            });
    }

private:
    boost::asio::ip::tcp::socket& _socket;
};

#endif  // TCP_WRITER_H
