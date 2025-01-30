#include "TcpClient.h"

#include "../exceptions/DisconnectException.h"

TcpClient::TcpClient(boost::asio::ip::tcp::socket socket)
    : _socket(std::move(socket))
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
