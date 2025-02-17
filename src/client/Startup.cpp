#include "Startup.h"

#include <thread>

#include "../common/network/tcp/Connector.h"
#include "../utils/Logger.h"

using namespace Client;

Startup::Startup(unsigned short port, std::string host)
    : _context(),
      _socket(_context),
      _messages_queue_handler(_socket),
      _messages_prompt(),
      _port(port),
      _host(host),
      _stop(false)
{
}

Startup::~Startup()
{
    Logger::info("Client stopped");

    _socket.close();
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

    while (!_stop)
    {
        _context.run();
    }
}

void Startup::run_user_thread()
{
    std::thread user_thread(
        [this]
        {
            while (!_stop)
            {
                std::vector<Message> messages = _messages_prompt.get();
                if (messages.empty())
                {
                    _stop = true;
                }

                _messages_queue_handler.handle(messages);
            }
        });

    user_thread.detach();  // the thread is detached from the main thread to run independently
}
