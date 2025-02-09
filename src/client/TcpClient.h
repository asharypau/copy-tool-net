#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <boost/asio.hpp>
#include <string>
#include <utility>

#include "../common/TcpWriter.h"
#include "../utils/Logger.h"

class TcpClient
{
public:
    explicit TcpClient(boost::asio::io_context& context, unsigned short port, std::string host);
    ~TcpClient();

    TcpClient(TcpClient&& other) noexcept;

    // Disallow copying and assignment.
    TcpClient(const TcpClient&) = delete;
    TcpClient& operator=(const TcpClient&) = delete;
    TcpClient& operator=(TcpClient&& other) = delete;

    TcpWriter& get_writer() noexcept
    {
        return _tcp_writer;
    }

    template <class TCallback>
    void connect(TCallback&& callback)
    {
        boost::asio::ip::tcp::resolver resolver(_socket.get_executor());
        boost::asio::ip::tcp::resolver::results_type endpoint = resolver.resolve(_host, std::to_string(_port));

        if (endpoint.empty())
        {
            Logger::error("Failed to resolve endpoint.");

            return;
        }

        boost::asio::async_connect(
            _socket,
            endpoint,
            [this, callback = std::forward<TCallback>(callback)](const boost::system::error_code& error, boost::asio::ip::tcp::endpoint)
            {
                if (error)
                {
                    Logger::error("Error during connect");
                    return;
                }

                try
                {
                    Logger::info("Connected by " + _host + ":" + std::to_string(_port));

                    callback();
                }
                catch (const std::exception& ex)
                {
                    Logger::error("Error occurred: " + std::string(ex.what()));
                }
            });
    }

private:
    boost::asio::ip::tcp::socket _socket;
    TcpWriter _tcp_writer;
    unsigned short _port;
    std::string _host;
};

#endif  // TCP_CLIENT_H
