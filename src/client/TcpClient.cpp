#include "TcpClient.h"

#include "../exceptions/DisconnectException.h"

TcpClient::TcpClient()
    : _context(),
      _socket(_context)
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

void TcpClient::connect(unsigned short port, const std::string& host)
{
    boost::asio::ip::tcp::resolver resolver(_context);
    boost::asio::ip::tcp::resolver::results_type endpoint = resolver.resolve(host, std::to_string(port));

    boost::asio::connect(_socket, endpoint);
}

void TcpClient::validate_result(boost::system::error_code& error, size_t size_in_bytes, size_t read_bytes)
{
    if (error == boost::asio::error::eof || error == boost::asio::error::connection_reset)
    {
        throw DisconnectException("The connection was broken");
    }

    if (read_bytes != size_in_bytes)
    {
        throw std::runtime_error("Unexpected bytes count received: " + std::to_string(read_bytes) + " instead of " + std::to_string(size_in_bytes));
    }
}
