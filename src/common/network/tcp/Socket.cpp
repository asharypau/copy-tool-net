#include "Socket.h"

#include <string>
#include <utility>

#include "../common/encryption/Aes.h"
#include "../common/encryption/Rsa.h"
#include "OperationException.h"

using namespace Tcp;

Socket::Socket(boost::asio::io_context& context)
    : _encryption_enabled(false),
      _socket(context),
      _read_buffer(),
      _symmetric_key(),
      _mtx()
{
}

Socket::Socket(boost::asio::ip::tcp::socket socket)
    : _encryption_enabled(false),
      _socket(std::move(socket)),
      _read_buffer(),
      _symmetric_key(),
      _mtx()
{
}

Socket::Socket(Socket&& other)
    : _encryption_enabled(other._encryption_enabled),
      _socket(std::move(other._socket)),
      _read_buffer(std::move(other._read_buffer)),
      _symmetric_key(std::move(other._symmetric_key)),
      _mtx()
{
}

Socket::~Socket()
{
    if (_socket.is_open())
    {
        _socket.close();
    }
}

bool Socket::is_open()
{
    std::unique_lock<std::mutex> lock(_mtx);

    return _socket.is_open();
}

void Socket::close()
{
    std::unique_lock<std::mutex> lock(_mtx);

    if (_socket.is_open())
    {
        _socket.close();
    }
}

Socket& Socket::operator=(Socket&& other)
{
    if (this == &other)
    {
        return *this;
    }

    _encryption_enabled = other._encryption_enabled;
    _socket = std::move(other._socket);
    _read_buffer = std::move(other._read_buffer);
    _symmetric_key = std::move(other._symmetric_key);

    return *this;
}

Socket::operator boost::asio::ip::tcp::socket&() noexcept
{
    return _socket;
}

boost::asio::awaitable<void> Socket::handshake(Tcp::HandshakeType type)
{
    switch (type)
    {
    case HandshakeType::SERVER:
        co_await server_handshake();
        break;
    case HandshakeType::CLIENT:
        co_await client_handshake();
        break;
    default:
        throw std::invalid_argument("Invalid handshake type");
    }

    _encryption_enabled = true;
}

boost::asio::awaitable<std::vector<unsigned char>> Socket::read_async()
{
    Tcp::header_t content_length = co_await read_header_async();
    std::vector<unsigned char> decrypted_bytes = read_data(content_length);

    co_return std::move(decrypted_bytes);
}

boost::asio::awaitable<void> Socket::write_async(const std::vector<unsigned char>& bytes)
{
    const std::vector<unsigned char> write_buffer = get_write_buffer(bytes);
    auto [error, _] = co_await boost::asio::async_write(
        _socket,
        boost::asio::buffer(write_buffer),
        boost::asio::as_tuple(boost::asio::use_awaitable));

    if (error)
    {
        throw Tcp::OperationException(error);
    }
}

boost::asio::awaitable<void> Socket::server_handshake()
{
    Encryption::asymmetric_key_t asymmetric_key = Encryption::Rsa::generate_key(2048);
    std::string public_asymmetric_key = Encryption::Rsa::extract_public_key(asymmetric_key);
    co_await write_async(std::vector<unsigned char>(public_asymmetric_key.begin(), public_asymmetric_key.end()));

    std::vector<unsigned char> encrypted_symmetric_key = co_await read_async();
    _symmetric_key = Encryption::Rsa::decrypt(asymmetric_key, encrypted_symmetric_key);
}

boost::asio::awaitable<void> Socket::client_handshake()
{
    std::vector<unsigned char> data = co_await read_async();
    Encryption::asymmetric_key_t public_asymmetric_key = Encryption::Rsa::to_public_key(std::string(data.begin(), data.end()));
    _symmetric_key = Encryption::Aes::generate_key(32);

    std::vector<unsigned char> encrypted_symmetric_key = Encryption::Rsa::encrypt(public_asymmetric_key, _symmetric_key);
    co_await write_async(encrypted_symmetric_key);
}

boost::asio::awaitable<Tcp::header_t> Socket::read_header_async()
{
    auto [error, _] = co_await boost::asio::async_read(
        _socket,
        _read_buffer.prepare(Tcp::HEADER_SIZE),
        boost::asio::transfer_exactly(Tcp::HEADER_SIZE),
        boost::asio::as_tuple(boost::asio::use_awaitable));

    if (error)
    {
        throw Tcp::OperationException(error);
    }

    _read_buffer.commit(Tcp::HEADER_SIZE);

    std::vector<unsigned char> extracted_bytes = extract_from_read_buffer(Tcp::HEADER_SIZE);

    Tcp::header_t header;
    std::memcpy(&header, extracted_bytes.data(), Tcp::HEADER_SIZE);

    co_return header;
}

std::vector<unsigned char> Socket::read_data(size_t content_length)
{
    boost::system::error_code error;
    boost::asio::read(_socket, _read_buffer.prepare(content_length), boost::asio::transfer_exactly(content_length), error);

    if (error)
    {
        throw Tcp::OperationException(error);
    }

    _read_buffer.commit(content_length);
    std::vector<unsigned char> extracted_bytes = extract_from_read_buffer(content_length);

    return _encryption_enabled
               ? std::move(Encryption::Aes::decrypt(_symmetric_key, extracted_bytes))
               : std::move(extracted_bytes);
}

std::vector<unsigned char> Socket::extract_from_read_buffer(size_t content_length)
{
    std::vector<unsigned char> extracted_bytes(content_length);
    unsigned char* begin = static_cast<unsigned char*>(_read_buffer.data().data());

    // copy bytes from read_buffer to extracted_bytes
    std::memcpy(extracted_bytes.data(), begin, content_length);

    _read_buffer.consume(content_length);

    return std::move(extracted_bytes);
}

std::vector<unsigned char> Socket::get_write_buffer(const std::vector<unsigned char>& data) const
{
    std::vector<unsigned char> bytes_to_write = _encryption_enabled ? Encryption::Aes::encrypt(_symmetric_key, data) : data;

    Tcp::header_t content_length = bytes_to_write.size();
    std::vector<unsigned char> write_buffer = std::vector<unsigned char>(Tcp::HEADER_SIZE + content_length);

    // copy content_length header
    std::memcpy(write_buffer.data(), &content_length, Tcp::HEADER_SIZE);

    // copy data
    std::memcpy(write_buffer.data() + Tcp::HEADER_SIZE, bytes_to_write.data(), content_length);

    return std::move(write_buffer);
}
