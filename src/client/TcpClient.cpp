#include "TcpClient.h"

TcpClient::TcpClient(boost::asio::io_context& context, unsigned short port, std::string host)
    : _socket(context),
      _tcp_writer(_socket),
      _port(port),
      _host(host)
{
}

TcpClient::~TcpClient()
{
    _socket.close();
}

TcpClient::TcpClient(TcpClient&& other) noexcept
    : _socket(std::move(other._socket)),
      _tcp_writer(other._tcp_writer),
      _port(other._port),
      _host(other._host)
{
}
