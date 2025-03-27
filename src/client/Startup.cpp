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
      _host(host)
{
}

void Startup::run()
{
    try
    {
        Logger::info("The client started");

        boost::asio::co_spawn(_context, connect(), boost::asio::detached);

        do
        {
            _context.run();
        } while (_socket.is_open());

        Logger::info("The client stopped");
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
        if (co_await handshake())
        {
            run_user_thread();
        }
    }
    else
    {
        Logger::error(std::format("Error occurred during connect {}: {}", op_result.error_code(), op_result.error_message()));
        stop();
    }
}

boost::asio::awaitable<bool> Startup::handshake()
{
    try
    {
        co_await _socket.handshake(Tcp::HandshakeType::CLIENT);

        co_return true;
    }
    catch (const std::exception& ex)
    {
        Logger::error(std::format("An error occurred during the client handshake: {}", ex.what()));
        stop();

        co_return false;
    }
}

void Startup::run_user_thread()
{
    std::thread user_thread(
        [this]
        {
            while (_socket.is_open())
            {
                std::vector<Message> messages = _messages_prompt.get();
                if (messages.empty())
                {
                    stop();
                }

                _messages_queue_handler.handle(messages);
            }
        });

    user_thread.detach();  // the thread is detached from the main thread to run independently
}

void Startup::stop()
{
    try
    {
        _socket.close();
    }
    catch (const std::exception& ex)
    {
        Logger::error(std::format("An error occurred during stopping the client: {}", ex.what()));
    }
}
