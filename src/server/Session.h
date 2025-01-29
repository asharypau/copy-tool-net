#ifndef SESSION_H
#define SESSION_H

#include <boost/asio.hpp>

class Session
{
public:
    explicit Session(boost::asio::ip::tcp::socket socket, size_t client_id);

    void start();

private:
    void read();
    size_t read_file_size();
    size_t read_batch_size();
    std::vector<char> read_batch(size_t batch_size);
    void validate_read(boost::system::error_code& error, size_t read_bytes, size_t expected_bytes);

    boost::asio::ip::tcp::socket _socket;
    size_t _client_id;
};

#endif  // SESSION_H
