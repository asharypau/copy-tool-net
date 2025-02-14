#ifndef SESSION_H
#define SESSION_H

#include <boost/asio.hpp>
#include <memory>
#include <string>

#include "../common/Tcp.h"
#include "Dispatcher.h"

namespace Server
{
    class Dispatcher;   // forward declaration
    class IController;  // forward declaration

    class Session : public std::enable_shared_from_this<Session>
    {
    public:
        Session(size_t client_id, boost::asio::ip::tcp::socket socket);
        ~Session();

        /**
         * @brief Runs a new session.
         */
        void run();

        /**
         * @brief Notifies that the request has been processed.
         */
        void notify_done();

    private:
        std::string _client_id;
        boost::asio::ip::tcp::socket _socket;
        Tcp::Reader _tcp_reader;
        Tcp::Writer _tcp_writer;
        Dispatcher _dispatcher;
        std::string _endpoint;
    };
}

#endif  // SESSION_H
