#include "Client.h"

#include <thread>

#include "../utils/Logger.h"

Client::Client(unsigned short port, std::string host)
    : _context(),
      _tcp_client(_context, port, host),
      _message_queue_handler(_tcp_client),
      _messages_prompt()
{
}

void Client::run()
{
    Logger::info("Client started");

    _tcp_client.connect(
        [this]
        {
            run_user_thread();
        });

    while (true)
    {
        _context.run();
    }
}

void Client::run_user_thread()
{
    std::thread user_thread(
        [this]
        {
            while (true)
            {
                std::vector<Message> messages = _messages_prompt.get();
                if (messages.empty())
                {
                    break;
                }

                _message_queue_handler.handle(messages);
            }
        });

    user_thread.detach();
}
