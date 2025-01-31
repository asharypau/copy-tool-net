#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <boost/asio.hpp>
#include <string>

class TcpClient
{
public:
    explicit TcpClient(boost::asio::io_context& context);
    TcpClient(TcpClient&&) noexcept;
    ~TcpClient();

    TcpClient& operator=(TcpClient&& other) noexcept;

    TcpClient(const TcpClient&) = delete;
    TcpClient& operator=(const TcpClient&) = delete;

    void connect(const std::string& host, unsigned short port);
    void accept(const std::string& host, unsigned short port);

    template <class TData>
    void write(TData* data, size_t size_in_bytes)
    {
        boost::system::error_code error;
        size_t read_bytes = boost::asio::write(_socket, boost::asio::buffer(data, size_in_bytes), error);

        validate_result(error, size_in_bytes, read_bytes);
    }

    template <class TData>
    void read(TData* data, size_t size_in_bytes)
    {
        boost::system::error_code error;
        size_t read_bytes = boost::asio::read(_socket, boost::asio::buffer(data, size_in_bytes), error);

        validate_result(error, size_in_bytes, read_bytes);
    }

private:
    void validate_result(boost::system::error_code& error, size_t size_in_bytes, size_t read_bytes);

    boost::asio::ip::tcp::socket _socket;
};

#endif  // TCP_CLIENT_H
