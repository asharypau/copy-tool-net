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
    void connect(const std::string& host, unsigned short port);

    /**
     * @brief Accepts a connection from the specified host and port.
     *
     * This method sets up the client to accept a connection from a remote server
     * using the provided hostname and port number. Once the connection is accepted,
     * the client will be able to send and receive data over the network.
     *
     * @param host The hostname or IP address of the remote server.
     * @param port The port number on the remote server to accept the connection from.
     */
    void accept(const std::string& host, unsigned short port);

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

    /**
     * @brief Reads data from the connected socket.
     *
     * This template method receives data from the network from the connected
     * remote server. The data is read as a raw byte stream.
     *
     * @tparam TData The type of the data to be read.
     * @param data A pointer to the buffer where the read data will be stored.
     * @param size_in_bytes The size of the buffer, in bytes.
     */
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
