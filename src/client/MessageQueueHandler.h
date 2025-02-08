#ifndef MESSAGE_QUEUE_HANDLER_H
#define MESSAGE_QUEUE_HANDLER_H

#include <mutex>
#include <queue>
#include <vector>

#include "../models/Message.h"
#include "FileReader.h"
#include "TcpClient.h"

class MessageQueueHandler
{
public:
    explicit MessageQueueHandler(TcpClient& tcp_client);

    void handle(std::vector<Message>& messages);

private:
    void send_headers(Message message);
    void send_file(std::unique_ptr<FileReader>&& file_reader);

    static constexpr size_t HEADER_SIZE = sizeof(size_t);
    static constexpr size_t BATCH_SIZE = 1024 * 1024;

    TcpClient& _tcp_client;
    std::queue<Message> _messages;
    std::vector<char> _header_buffer;
    std::vector<char> _data_buffer;
    std::mutex _mtx;
    bool _in_progress;
};

#endif  // MESSAGE_QUEUE_HANDLER_H
