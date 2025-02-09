#include "Server.h"

#include "../utils/Logger.h"
#include "Session.h"

size_t Server::_client_id = 0;

Server::Server(unsigned short port)
    : _context(),
      _acceptor(port, _context)
{
}

void Server::run()
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

void Server::accept()
{
    _acceptor.accept(
        [this](boost::asio::ip::tcp::socket socket)
        {
            ++_client_id;
            std::make_shared<Session>(std::move(socket), _client_id)->run();

            accept();  // wait for another connection
        });
}
