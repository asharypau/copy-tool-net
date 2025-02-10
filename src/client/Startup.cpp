#include "Startup.h"

#include <thread>

#include "../common/Tcp.h"
#include "../utils/Logger.h"

using namespace Client;

Startup::Startup(unsigned short port, std::string host)
    : _context(),
      _socket(_context),
      _message_queue_handler(Tcp::Writer(_socket)),
      _messages_prompt(),
      _port(port),
      _host(host)
{
}

void Startup::run()
{
    Logger::info("Client started");

    Tcp::Connector::connect(
        _port,
        _host,
        _socket,
        [this]
        {
            run_user_thread();
        });

    while (true)
    {
        _context.run();
    }
}

void Startup::run_user_thread()
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
