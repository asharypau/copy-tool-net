#ifndef SESSION_H
#define SESSION_H

#include <boost/asio.hpp>

#include "../exceptions/DisconnectException.h"

class Session
{
public:
    explicit Session(boost::asio::ip::tcp::socket socket, size_t client_id);

    void start();

private:
    void read_file();
    size_t read_size();
    std::vector<char> read_batch(size_t batch_size);

    template <class TData>
    void read(TData* data, size_t size_in_bytes)
    {
        boost::system::error_code error;
        size_t read_bytes = boost::asio::read(_socket, boost::asio::buffer(data, size_in_bytes), error);

        if (error == boost::asio::error::eof || error == boost::asio::error::connection_reset)
        {
            throw DisconnectException("Client disconnected: " + std::to_string(_client_id));
        }

        if (read_bytes != size_in_bytes)
        {
            throw std::runtime_error("Unexpected bytes count received: " + std::to_string(read_bytes) + " instead of " + std::to_string(size_in_bytes));
        }
    }

    boost::asio::ip::tcp::socket _socket;
    size_t _client_id;
};

#endif  // SESSION_H
