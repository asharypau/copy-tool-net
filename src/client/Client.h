#ifndef CLIENT_H
#define CLIENT_H

#include <boost/asio.hpp>
#include <string>

#include "../common/IApplication.h"
#include "MessageQueueHandler.h"
#include "MessagesPrompt.h"
#include "TcpClient.h"

class Client : public IApplication
{
public:
    Client(unsigned short port, std::string host);

    void run() override;

private:
    void run_user_thread();

    boost::asio::io_context _context;
    TcpClient _tcp_client;
    MessageQueueHandler _message_queue_handler;
    MessagesPrompt _messages_prompt;
};

#endif  // CLIENT_H
