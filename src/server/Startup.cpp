#include "Startup.h"

#include <filesystem>
#include <string>

#include "../utils/Logger.h"

using namespace Server;

Startup::Startup(unsigned short port)
    : _context(),
      _acceptor(port, _context),
      _session_manager(_context)
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

        ++CLIENT_ID;
        create_client_storage();
        _session_manager.start_new(CLIENT_ID, std::move(socket));
    }
}

void Startup::create_client_storage()
{
    std::string path = std::string(Server::CLIENT_STORAGE_PATH) + std::to_string(CLIENT_ID);
    std::filesystem::create_directory(path);
}
