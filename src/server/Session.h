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
         * @tparam TCompletionCallback The type of the completion callback function.
         * @param completion_callback The callback to be executed upon completion of the operation.
         */
        template <class TCompletionCallback>
        boost::asio::awaitable<void> run(TCompletionCallback completion_callback)
        {
            Logger::info(std::format("Session run for the client {}", _client_id));

            if (co_await handshake())
            {
                co_await run_request_processing_loop();
            }

            completion_callback(_client_id);
        }

    private:
        /**
         * @brief Performs an asynchronous server handshake.
         *
         * @return boost::asio::awaitable<bool> `true` if the handshake is successful, `false` if an error occurs.
         */
        boost::asio::awaitable<bool> handshake();

        /**
         * @brief Runs the request processing loop asynchronously.
         */
        boost::asio::awaitable<void> run_request_processing_loop();

        /**
         * @brief Closes the socket.
         */
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
