#include "Startup.h"

#include <format>
#include <string>

#include "../utils/Logger.h"

using namespace Server;

Startup::Startup(unsigned short port)
    : _context(),
      _acceptor(port, _context),
      _session_manager(_context),
      _client_id(0)
{
}

void Startup::run()
{
    try
    {
        Logger::info("Server started");

        boost::asio::co_spawn(_context, accept(), boost::asio::detached);
        _context.run();

        Logger::info("Server stopped");
    }
    catch (const std::exception& ex)
    {
        Logger::error(ex.what());
    }
}

boost::asio::awaitable<void> Startup::accept()
{
    while (true)
    {
        auto op_result = co_await _acceptor.accept();
        if (op_result)
        {
            ++_client_id;
            _session_manager.start_new(_client_id, std::move(op_result.acquire_result()));

            if (op_result)
            {
                Logger::info("true");
            }
        }
        else
        {
            Logger::info(std::format("The client is not accepted, {}: {}", op_result.error_code(), op_result.error_message()));
        }
    }
}
