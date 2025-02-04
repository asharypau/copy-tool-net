#ifndef SESSION_H
#define SESSION_H

#include <boost/asio.hpp>
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "FileWriter.h"

class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(boost::asio::ip::tcp::socket socket, size_t client_id);

    void start();

private:
    void read_file_size();
    void read_file_name_size();
    void read_file_name();
    void read_batch_size();
    void read_batch();
    void handle_error(const boost::system::error_code& error);

    boost::asio::ip::tcp::socket _socket;
    std::string _file_name;
    std::vector<char> _batch_data;
    FileWriter _file;

    size_t _client_id;
    size_t _file_size;
    size_t _file_name_size;
    size_t _batch_size;
};

#endif  // SESSION_H
