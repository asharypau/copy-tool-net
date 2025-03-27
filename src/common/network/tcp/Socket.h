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

        bool is_open();
        void close();

        boost::asio::awaitable<void> handshake(Tcp::HandshakeType type);
        boost::asio::awaitable<std::vector<unsigned char>> read_async();
        boost::asio::awaitable<void> write_async(const std::vector<unsigned char>& bytes);

        boost::asio::ip::tcp::socket& get() noexcept
        {
            return _socket;
        }

        const boost::asio::basic_socket<boost::asio::ip::tcp>::executor_type& get_executor() noexcept
        {
            return _socket.get_executor();
        }

    private:
        boost::asio::awaitable<void> server_handshake();
        boost::asio::awaitable<void> client_handshake();

        boost::asio::awaitable<Tcp::header_t> read_header_async();
        std::vector<unsigned char> read_data(size_t content_length);
        std::vector<unsigned char> extract_from_read_buffer(size_t content_length);

        std::vector<unsigned char> get_write_buffer(const std::vector<unsigned char>& data) const;

        bool _encryption_enabled;
        boost::asio::ip::tcp::socket _socket;
        boost::beast::flat_buffer _read_buffer;
        Encryption::symmetric_key_t _symmetric_key;
        std::mutex _mtx;
    };
}

#endif  // TCP_SOCKET_H
