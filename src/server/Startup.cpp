#include "Startup.h"

#include "../utils/Logger.h"
#include "Session.h"

using namespace Server;

size_t Startup::_client_id = 0;

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
            ++_client_id;
            std::make_shared<Session>(std::move(socket), _client_id)->run();

            accept();  // wait for another connection
        });
}
