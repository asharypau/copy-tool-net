#ifndef CLIENT_STARTUP_H
#define CLIENT_STARTUP_H

#include <boost/asio.hpp>
#include <string>

#include "../common/IApplication.h"
#include "MessageQueueHandler.h"
#include "MessagesPrompt.h"

namespace Client
{
    class Startup : public IApplication
    {
    public:
        Startup(unsigned short port, std::string host);

        void run() override;

    private:
        void run_user_thread();

        boost::asio::io_context _context;
        boost::asio::ip::tcp::socket _socket;
        MessageQueueHandler _message_queue_handler;
        MessagesPrompt _messages_prompt;
        unsigned short _port;
        std::string _host;
    };
}

#endif  // CLIENT_STARTUP_H
