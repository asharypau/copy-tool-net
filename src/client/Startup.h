#ifndef CLIENT_STARTUP_H
#define CLIENT_STARTUP_H

#include <boost/asio.hpp>
#include <string>

#include "../infrastructure/IApplication.h"
#include "MessagesPrompt.h"
#include "MessagesQueueHandler.h"
#include "boost/asio/awaitable.hpp"

namespace Client
{
    class Startup : public IApplication
    {
    public:
        Startup(unsigned short port, std::string host);
        ~Startup();

        void run() override;

    private:
        boost::asio::awaitable<void> connect();
        /**
         * @brief Starts a new thread to handle user messages asynchronously.
         */
        void run_user_thread();

        boost::asio::io_context _context;
        boost::asio::ip::tcp::socket _socket;
        MessagesQueueHandler _messages_queue_handler;
        MessagesPrompt _messages_prompt;
        unsigned short _port;
        std::string _host;
        bool _stop;  // should be atomic
    };
}

#endif  // CLIENT_STARTUP_H
