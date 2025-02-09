#ifndef SESSION_H
#define SESSION_H

#include <boost/asio.hpp>
#include <memory>

#include "../common/Tcp.h"
#include "FileHandler.h"

class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(boost::asio::ip::tcp::socket socket, size_t client_id);

    void run();

private:
    void get_headers();
    void get_file(std::shared_ptr<FileHandler> file_handler);

    static constexpr size_t HEADER_SIZE = sizeof(size_t);

    boost::asio::ip::tcp::socket _socket;
    Tcp::Reader _tcp_reader;
    size_t _client_id;
};

#endif  // SESSION_H
