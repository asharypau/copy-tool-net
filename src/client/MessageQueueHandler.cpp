#include "MessageQueueHandler.h"

#include <string>

MessageQueueHandler::MessageQueueHandler(TcpClient& tcp_client)
    : _tcp_client(tcp_client),
      _messages(),
      _header_buffer(),
      _data_buffer(OFFSET + BATCH_SIZE),
      _in_progress(false)
{
}

void MessageQueueHandler::handle(std::vector<Message>& messages)
{
    for (const Message& message : messages)
    {
        _messages.push_back(std::move(message));
    }

    if (!_in_progress)
    {
        _in_progress = true;
        send_header();
    }
}

void MessageQueueHandler::send_header()
{
    Message& file_info = _messages.front();
    size_t file_name_size = file_info.name.size();

    _header_buffer.resize(OFFSET + OFFSET + file_name_size);
    std::memcpy(_header_buffer.data(), &file_info.size, OFFSET);                                  // write a file_reader size into the buffer at index 0
    std::memcpy(_header_buffer.data() + OFFSET, &file_name_size, OFFSET);                         // write a file_reader name size into the buffer at index 0 + SIZE
    std::memcpy(_header_buffer.data() + OFFSET + OFFSET, file_info.name.data(), file_name_size);  // write a name into the buffer at index 0 + SIZE + SIZE

    _tcp_client.write(
        _header_buffer.data(),
        _header_buffer.size(),
        [this]
        {
            Message& file_info = _messages.front();
            send_file(std::make_unique<FileReader>(file_info.path));
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

        _messages.pop_front();
        _in_progress = false;

        if (!_messages.empty())
        {
            send_header();
        }
    }
}
