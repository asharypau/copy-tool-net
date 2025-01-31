#include "TcpClient.h"

#include <string>

#include "../exceptions/DisconnectException.h"

TcpClient::TcpClient(boost::asio::io_context& context)
    : _socket(context)
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

void TcpClient::connect(const std::string& host, unsigned short port)
{
    boost::asio::ip::tcp::resolver resolver(_socket.get_executor());
    boost::asio::ip::tcp::resolver::results_type endpoint = resolver.resolve(host, std::to_string(port));

    boost::asio::connect(_socket, endpoint);
}

void TcpClient::accept(const std::string& host, unsigned short port)
{
    // boost::asio::ip::make_address(host)
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port);
    boost::asio::ip::tcp::acceptor acceptor(_socket.get_executor(), endpoint);

    acceptor.accept(_socket);
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
