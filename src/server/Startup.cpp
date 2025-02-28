#include "Startup.h"

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
        auto socket = co_await _acceptor.accept();

        ++_client_id;
        _session_manager.start_new(_client_id, std::move(socket));
    }
}
