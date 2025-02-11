#ifndef SERVER_SESSION_H
#define SERVER_SESSION_H

#include <boost/asio.hpp>
#include <memory>
#include <vector>

#include "../common/Tcp.h"
#include "FileHandler.h"

namespace Server
{
    class Session : public std::enable_shared_from_this<Session>
    {
    public:
        Session(boost::asio::ip::tcp::socket socket, size_t client_id);

        void run();

    private:
        void get_headers();
        void get_file(std::unique_ptr<FileHandler>&& file);

        static constexpr size_t HEADER_SIZE = sizeof(size_t);

        boost::asio::ip::tcp::socket _socket;
        Tcp::Reader _tcp_reader;
        std::string _file_name_buffer;
        std::vector<char> _data_buffer;
        size_t _client_id;
    };
}

#endif  // SERVER_SESSION_H
