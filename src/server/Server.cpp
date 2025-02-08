#include "Server.h"

#include "../utils/Logger.h"
#include "Session.h"

size_t Server::_client_id = 0;

Server::Server(unsigned short port)
    : _context(),
      _acceptor(_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
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
    _acceptor.async_accept(
        [this](const boost::system::error_code& error, boost::asio::ip::tcp::socket socket)
        {
            if (!error)
            {
                ++_client_id;
                std::make_shared<Session>(std::move(socket), _client_id)->run();
            }

            accept();  // wait for another connection
        });
}
