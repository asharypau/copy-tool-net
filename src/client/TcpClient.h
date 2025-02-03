#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <boost/asio.hpp>
#include <string>

class TcpClient
{
public:
    TcpClient();
    TcpClient(TcpClient&&) noexcept;
    ~TcpClient();

    TcpClient& operator=(TcpClient&& other) noexcept;

    // Disallow copying and assignment.
    TcpClient(const TcpClient&) = delete;
    TcpClient& operator=(const TcpClient&) = delete;

    /**
     * @brief Establishes a connection to the specified host and port.
     *
     * This method attempts to connect to a remote server using the provided
     * hostname and port number. If the connection is successful, the client
     * will be able to send and receive data over the network.
     *
     * @param host The hostname or IP address of the remote server.
     * @param port The port number on the remote server to connect to.
     */
    void connect(unsigned short port, const std::string& host);

    /**
     * @brief Writes data to the connected socket.
     *
     * This template method sends the specified data over the network to the
     * connected remote server. The data is sent as a raw byte stream.
     *
     * @tparam TData The type of the data to be written.
     * @param data A pointer to the data to be written.
     * @param size_in_bytes The size of the data to be written, in bytes.
     */
    template <class TData>
    void write(TData* data, size_t size_in_bytes)
    {
        boost::system::error_code error;
        size_t read_bytes = boost::asio::write(_socket, boost::asio::buffer(data, size_in_bytes), error);

        validate_result(error, size_in_bytes, read_bytes);
    }

private:
    void validate_result(boost::system::error_code& error, size_t size_in_bytes, size_t read_bytes);

    boost::asio::io_context _context;
    boost::asio::ip::tcp::socket _socket;
};

#endif  // TCP_CLIENT_H
