#ifndef SESSION_H
#define SESSION_H

#include <boost/asio.hpp>

#include "../common/network/tcp/Reader.h"
#include "../common/network/tcp/Socket.h"
#include "../common/network/tcp/Writer.h"
#include "../utils/Logger.h"
#include "Dispatcher.h"
#include "boost/asio/awaitable.hpp"
#include "services/FileStorage.h"

namespace Server
{
    class Session
    {
    public:
        Session(const std::size_t client_id, Tcp::Socket socket);

        /**
         * @brief Runs a new session.
         */
        template <class TCallback>
        boost::asio::awaitable<void> run(TCallback callback)
        {
            Logger::info(std::format("Session run for the client {}", _client_id));

            if (co_await handshake())
            {
                co_await run_request_processing_loop();
            }

            callback(_client_id);
        }

    private:
        boost::asio::awaitable<bool> handshake();
        boost::asio::awaitable<void> run_request_processing_loop();
        void stop();

        Tcp::Socket _socket;
        Tcp::Reader _tcp_reader;
        Tcp::Writer _tcp_writer;
        FileStorage _file_storage;
        Dispatcher _dispatcher;
        const std::size_t _client_id;
    };
}

#endif  // SESSION_H
