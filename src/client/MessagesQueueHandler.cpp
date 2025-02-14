#include "MessagesQueueHandler.h"

#include <algorithm>
#include <string>
#include <vector>

using namespace Client;

MessagesQueueHandler::MessagesQueueHandler(boost::asio::ip::tcp::socket& socket)
    : _messages(),
      _pending_messages(),
      _file_client(socket),
      _writing_in_progress(false),
      _reading_in_progress(false)
{
    _file_client.register_write_handler(
        [this]
        { write_handle(); });

    _file_client.register_read_handler(
        [this](size_t id)
        { read_handle(id); });
}

void MessagesQueueHandler::handle(std::vector<Message>& messages)
{
    std::unique_lock<std::mutex> lock(_mtx);

    for (const Message& message : messages)
    {
        _messages.push(std::move(message));
    }

    if (!_writing_in_progress)
    {
        _writing_in_progress = true;
        _file_client.write(_messages.front());
    }

    if (!_reading_in_progress)
    {
        _reading_in_progress = true;
        _file_client.read();
    }
}

void MessagesQueueHandler::write_handle()
{
    std::unique_lock<std::mutex> lock(_mtx);

    Message& message = _messages.front();
    Logger::info("The message " + message.name + ':' + message.path + " has been sent.");

    _pending_messages.push_back(std::move(_messages.front()));
    _messages.pop();
    _writing_in_progress = false;

    if (!_messages.empty())
    {
        _writing_in_progress = true;
        _file_client.write(_messages.front());
    }
}

void MessagesQueueHandler::read_handle(size_t id)
{
    std::unique_lock<std::mutex> lock(_mtx);

    if (_pending_messages.empty())
    {
        Logger::warning("Attempt to remove message by id:" + std::to_string(id) + " from empty queue.");
        return;
    }

    std::vector<Message>::iterator current = std::find_if(
        _pending_messages.begin(),
        _pending_messages.end(),
        [id](const Message& current)
        {
            return current.id == id;
        });

    if (current != _pending_messages.end())
    {
        Logger::info("Confirmation of successful message " + current->name + ':' + current->path + " sending received.");

        _pending_messages.erase(current);
    }

    _file_client.read();
}
