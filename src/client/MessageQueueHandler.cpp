#include "MessageQueueHandler.h"

#include <string>

MessageQueueHandler::MessageQueueHandler(TcpClient& tcp_client)
    : _tcp_client(tcp_client),
      _messages(),
      _header_buffer(),
      _data_buffer(OFFSET + BATCH_SIZE),
      _mtx(),
      _in_progress(false)
{
}

void MessageQueueHandler::handle(std::vector<Message>& messages)
{
    std::unique_lock<std::mutex> lock(_mtx);
    for (const Message& message : messages)
    {
        _messages.push(std::move(message));
    }

    if (!_in_progress)
    {
        _in_progress = true;
        send_header(_messages.front());
    }
}

void MessageQueueHandler::send_header(Message message)
{
    size_t file_name_size = message.name.size();

    _header_buffer.resize(OFFSET + OFFSET + file_name_size);
    std::memcpy(_header_buffer.data(), &message.size, OFFSET);                                  // write a file_reader size into the buffer at index 0
    std::memcpy(_header_buffer.data() + OFFSET, &file_name_size, OFFSET);                       // write a file_reader name size into the buffer at index 0 + SIZE
    std::memcpy(_header_buffer.data() + OFFSET + OFFSET, message.name.data(), file_name_size);  // write a name into the buffer at index 0 + SIZE + SIZE

    _tcp_client.write(
        _header_buffer.data(),
        _header_buffer.size(),
        [this, message]
        {
            send_file(std::make_unique<FileReader>(message.path));
        });
}

void MessageQueueHandler::send_file(std::unique_ptr<FileReader>&& file_reader)
{
    size_t bytes_read = file_reader->read(_data_buffer.data() + OFFSET, BATCH_SIZE);  // write data into the buffer at index 0 + SIZE
    if (bytes_read > 0)
    {
        std::memcpy(_data_buffer.data(), &bytes_read, OFFSET);  // write batch size into the buffer at index 0

        _tcp_client.write(
            _data_buffer.data(),
            bytes_read + OFFSET,
            [this, file_reader = std::move(file_reader)]() mutable
            {
                send_file(std::move(file_reader));
            });
    }
    else
    {
        //  Message& file_info = _messages.front();
        // Logger::info("File sent: " + file_info.name + " (" + std::to_string(file_info.size) + " bytes)");

        std::unique_lock<std::mutex> lock(_mtx);

        _messages.pop();
        _in_progress = false;

        if (!_messages.empty())
        {
            _in_progress = true;
            send_header(_messages.front());
        }
    }
}
