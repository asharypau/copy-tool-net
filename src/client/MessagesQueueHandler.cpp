#include "MessagesQueueHandler.h"

#include <algorithm>
#include <vector>

using namespace Client;

MessagesQueueHandler::MessagesQueueHandler(boost::asio::ip::tcp::socket& socket)
    : _messages(),
      _pending_messages(),
      _message_handler(socket),
      _writing_in_progress(false),
      _reading_in_progress(false)
{
    _message_handler.register_write_handler(
        [this]
        { write_handle(); });

    _message_handler.register_read_handler(
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
        _message_handler.write(_messages.front());
    }

    if (!_reading_in_progress)
    {
        _reading_in_progress = true;
        _message_handler.read();
    }
}

void MessagesQueueHandler::write_handle()
{
    std::unique_lock<std::mutex> lock(_mtx);

    _pending_messages.push_back(std::move(_messages.front()));
    _messages.pop();
    _writing_in_progress = false;

    if (!_messages.empty())
    {
        _writing_in_progress = true;
        _message_handler.write(_messages.front());
    }
}

void MessagesQueueHandler::read_handle(size_t id)
{
    std::unique_lock<std::mutex> lock(_mtx);

    if (_pending_messages.empty())
    {
        Logger::warning("Attempt to remove from empty queue");
        return;
    }

    std::vector<Message>::iterator it = std::find_if(
        _pending_messages.begin(),
        _pending_messages.end(),
        [id](const Message& current)
        {
            return current.id == id;
        });

    if (it != _pending_messages.end())
    {
        _pending_messages.erase(it);
    }

    _reading_in_progress = false;

    if (!_pending_messages.empty())
    {
        _reading_in_progress = true;
        _message_handler.read();
    }
}
