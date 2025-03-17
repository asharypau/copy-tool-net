#include "MessagesQueueHandler.h"

#include <algorithm>
#include <optional>
#include <vector>

#include "../utils/Logger.h"

using namespace Client;

MessagesQueueHandler::MessagesQueueHandler(boost::asio::ip::tcp::socket& socket)
    : _socket(socket),
      _messages(),
      _pending_messages(),
      _file_client(socket),
      _writing_in_progress(false),
      _reading_in_progress(false)
{
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
        boost::asio::co_spawn(_socket.get_executor(), write(), boost::asio::detached);
    }
}

boost::asio::awaitable<void> MessagesQueueHandler::write()
{
    bool stop = false;

    while (!stop)
    {
        try
        {
            std::optional<Message> message;
            {
                std::unique_lock<std::mutex> lock(_mtx);
                message = _messages.front();
            }

            co_await _file_client.write(*message);
            Logger::info("The message " + message->name + ':' + message->path + " has been sent.");

            {
                std::unique_lock<std::mutex> lock(_mtx);

                _pending_messages.push_back(std::move(*message));
                _messages.pop();

                stop = _messages.empty();

                if (!_reading_in_progress)
                {
                    _reading_in_progress = true;
                    boost::asio::co_spawn(_socket.get_executor(), read_confirmation(), boost::asio::detached);
                }
            }
        }
        catch (const Tcp::OperationException& ex)
        {
            Logger::error(std::format("An error occurred during writing the message {}: {}", ex.error_code(), ex.what()));

            if (ex.error_code() == boost::asio::error::eof || ex.error_code() == boost::asio::error::connection_reset)
            {
                stop = true;
            }
        }
        catch (const std::exception& ex)
        {
            Logger::error(std::format("An error occurred during writing the message: {}", ex.what()));
            stop = true;
        }
    }

    std::unique_lock<std::mutex> lock(_mtx);
    _writing_in_progress = false;
}

boost::asio::awaitable<void> MessagesQueueHandler::read_confirmation()
{
    bool stop = false;

    while (!stop)
    {
        try
        {
            Tcp::header_t confirmation_id = co_await _file_client.read_confirmation();

            {
                std::unique_lock<std::mutex> lock(_mtx);

                std::vector<Message>::iterator it = std::find_if(
                    _pending_messages.begin(),
                    _pending_messages.end(),
                    [confirmation_id](const Message& current)
                    {
                        return current.id == confirmation_id;
                    });

                if (it != _pending_messages.end())
                {
                    Logger::info("Confirmation of successful message " + it->name + ':' + it->path + " sending received.");

                    _pending_messages.erase(it);
                }

                stop = _pending_messages.empty();
            }
        }
        catch (const Tcp::OperationException& ex)
        {
            Logger::error(std::format("An error occurred during writing the message {}: {}", ex.error_code(), ex.what()));

            if (ex.error_code() == boost::asio::error::eof || ex.error_code() == boost::asio::error::connection_reset)
            {
                stop = true;
            }
        }
        catch (const std::exception& ex)
        {
            Logger::error(std::format("An error occurred during writing the message: {}", ex.what()));
            stop = true;
        }
    }

    std::unique_lock<std::mutex> lock(_mtx);
    _reading_in_progress = false;
}
