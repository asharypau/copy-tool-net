#include "Startup.h"

#include <format>
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
    try
    {
        _socket.close();
    }
    catch (const std::exception& ex)
    {
        Logger::error(std::format("An error occurred during client destruction: {}", ex.what()));
    }
}

void Startup::run()
{
    try
    {
        Logger::info("Client started");

        boost::asio::co_spawn(_context, connect(), boost::asio::detached);
        while (!_stop)
        {
            _context.run();
        }

        Logger::info("Client stopped");
    }
    catch (const std::exception& ex)
    {
        Logger::error(ex.what());
    }
}

boost::asio::awaitable<void> Startup::connect()
{
    Tcp::OperationResult<bool> op_result = co_await Tcp::Connector::connect(_port, _host, _socket);
    if (op_result)
    {
        run_user_thread();
    }
    else
    {
        _stop = true;
        Logger::error(std::format("Error occurred during connect {}: {}", op_result.error_code(), op_result.error_message()));
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
