#include "TcpClient.h"

TcpClient::TcpClient(boost::asio::io_context& context, unsigned short port, std::string host)
    : _socket(context),
      _port(port),
      _host(host)
{
}

TcpClient::TcpClient(TcpClient&& other) noexcept
    : _socket(std::move(other._socket))
{
}

TcpClient::~TcpClient()
{
    _socket.close();
}

TcpClient& TcpClient::operator=(TcpClient&& other) noexcept
{
    if (this != &other)
    {
        _socket = std::move(other._socket);
    }

    return *this;
}