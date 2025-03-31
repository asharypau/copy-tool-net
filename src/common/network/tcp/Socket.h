#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <mutex>
#include <vector>

#include "../common/encryption/Constants.h"
#include "Constants.h"

namespace Tcp
{
    class Socket
    {
    public:
        explicit Socket(boost::asio::io_context& context);
        Socket(boost::asio::ip::tcp::socket socket);
        Socket(Socket&& other);
        Socket(const Socket&) = delete;  // Disallow copying
        ~Socket();

        Socket& operator=(Socket&& other);
        operator boost::asio::ip::tcp::socket&() noexcept;
        Socket& operator=(const Socket&) = delete;  // Disallow copy assignment

        /**
         * @brief Checks if the socket is open.
         *
         * @return bool `true` if the socket is open, `false` if the socket is closed.
         */
        bool is_open();

        /**
         * @brief Closes the socket.
         */
        void close();

        /**
         * @brief Performs a handshake for the given connection type.
         *
         * @param type The type of handshake to perform, represented by `Tcp::HandshakeType`
         * (e.g., CLIENT or SERVER).
         *
         * @throws std::invalid_argument If the type is invalid.
         */
        boost::asio::awaitable<void> handshake(Tcp::HandshakeType type);

        /**
         * @brief Asynchronously reads data.
         *
         * @return boost::asio::awaitable<std::vector<unsigned char>> An awaitable coroutine that
         * returns a `std::vector<unsigned char>` containing the data that was read.
         *
         * @throws Tcp::OperationException If an error occurs during the read operation (e.g., network failure,
         * timeout, or invalid data format), the exception will be thrown and should be handled
         * by the caller.
         */
        boost::asio::awaitable<std::vector<unsigned char>> read_async();

        /**
         * @brief Asynchronously writes data.
         *
         * @param bytes A constant reference to a `std::vector<unsigned char>` containing the data to be written.
         *
         * @throws Tcp::OperationException If an error occurs during the read operation (e.g., network failure,
         * timeout, or invalid data format), the exception will be thrown and should be handled
         * by the caller.
         */
        boost::asio::awaitable<void> write_async(const std::vector<unsigned char>& bytes);

        /**
         * @brief Retrieves the underlying TCP socket.
         *
         * @return boost::asio::ip::tcp::socket& A reference to the `boost::asio::ip::tcp::socket`.
         */
        boost::asio::ip::tcp::socket& get() noexcept
        {
            return _socket;
        }

        /**
         * @brief Retrieves the executor associated with the socket.
         *
         * @return const boost::asio::basic_socket<boost::asio::ip::tcp>::executor_type& A reference
         * to the executor associated with the socket.
         */
        const boost::asio::basic_socket<boost::asio::ip::tcp>::executor_type& get_executor() noexcept
        {
            return _socket.get_executor();
        }

    private:
        boost::asio::awaitable<void> server_handshake();
        boost::asio::awaitable<void> client_handshake();

        boost::asio::awaitable<Tcp::header_t> read_header_async();
        std::vector<unsigned char> read_data(std::size_t content_length);
        std::vector<unsigned char> extract_from_read_buffer(std::size_t content_length);

        std::vector<unsigned char> get_write_buffer(const std::vector<unsigned char>& data) const;

        bool _encryption_enabled;
        boost::asio::ip::tcp::socket _socket;
        boost::beast::flat_buffer _read_buffer;
        Encryption::symmetric_key_t _symmetric_key;
        std::mutex _mtx;
    };
}

#endif  // TCP_SOCKET_H
