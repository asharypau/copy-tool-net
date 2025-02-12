#include "Startup.h"

#include <filesystem>
#include <string>

#include "../utils/Logger.h"
#include "Session.h"

using namespace Server;

Startup::Startup(unsigned short port)
    : _context(),
      _acceptor(port, _context)
{
}

void Startup::run()
{
    try
    {
        Logger::info("Server started");

        accept();
        _context.run();

        Logger::info("Server stopped");
    }
    catch (const std::exception& ex)
    {
        Logger::error(ex.what());
    }
}

void Startup::accept()
{
    _acceptor.accept(
        [this](boost::asio::ip::tcp::socket socket)
        {
            ++CLIENT_ID;
            create_client_storage();
            std::make_shared<Session>(std::move(socket), CLIENT_ID)->run();

            accept();  // wait for another connection
        });
}

void Startup::create_client_storage()
{
    std::filesystem::create_directory(std::string(Server::CLIENT_STORAGE_PATH) + std::to_string(CLIENT_ID));
}
