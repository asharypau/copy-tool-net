#ifndef CLIENT_MESSAGE_QUEUE_HANDLER_H
#define CLIENT_MESSAGE_QUEUE_HANDLER_H

#include <mutex>
#include <queue>
#include <vector>

#include "../common/Tcp.h"
#include "../models/Message.h"
#include "FileHandler.h"

namespace Client
{
    class MessagesQueueHandler
    {
    public:
        explicit MessagesQueueHandler(Tcp::Writer tcp_writer);

        void handle(std::vector<Message>& messages);

    private:
        void send_headers(Message message);
        void send_file(std::unique_ptr<FileHandler>&& file);

        static constexpr size_t HEADER_SIZE = sizeof(size_t);
        static constexpr size_t BATCH_SIZE = 1024 * 1024;

        std::queue<Message> _messages;
        std::vector<char> _header_buffer;
        std::vector<char> _data_buffer;
        std::mutex _mtx;
        Tcp::Writer _tcp_writer;
        bool _in_progress;
    };
}

#endif  // CLIENT_MESSAGE_QUEUE_HANDLER_H
