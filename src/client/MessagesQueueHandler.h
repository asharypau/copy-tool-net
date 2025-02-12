#ifndef CLIENT_MESSAGE_QUEUE_HANDLER_H
#define CLIENT_MESSAGE_QUEUE_HANDLER_H

#include <mutex>
#include <queue>
#include <vector>

#include "../models/Message.h"
#include "MessageHandler.h"

namespace Client
{
    class MessagesQueueHandler
    {
    public:
        explicit MessagesQueueHandler(boost::asio::ip::tcp::socket& socket);

        void handle(std::vector<Message>& messages);

    private:
        void write_handle();
        void read_handle(size_t id);

        std::queue<Message> _messages;
        std::vector<Message> _pending_messages;
        MessageHandler _message_handler;
        std::mutex _mtx;
        bool _writing_in_progress;
        bool _reading_in_progress;
    };
}

#endif  // CLIENT_MESSAGE_QUEUE_HANDLER_H
