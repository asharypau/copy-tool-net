#ifndef SERVER_SESSION_H
#define SERVER_SESSION_H

#include <boost/asio.hpp>
#include <memory>
#include <string>

#include "../common/Tcp.h"
#include "FileHandler.h"
#include "MessageReader.h"

namespace Server
{
    class Session : public std::enable_shared_from_this<Session>
    {
    public:
        Session(boost::asio::ip::tcp::socket socket, size_t client_id);
        ~Session();

        /**
         * @brief Runs a new session.
         */
        void run();

    private:
        /**
         * @brief Writes a confirmation via the TCP writer.
         */
        void write_confirmation(std::shared_ptr<FileHandler>&& file);

        boost::asio::ip::tcp::socket _socket;
        MessageReader _message_reader;
        Tcp::Writer _tcp_writer;
        std::string _client_id;
    };
}

#endif  // SERVER_SESSION_H
