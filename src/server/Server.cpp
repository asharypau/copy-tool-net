#include "Server.h"

#include "../utils/Logger.h"
#include "ServerSession.h"

void Server::run(CmdArgs cmd_args)
{
    try
    {
        Logger::info("Server started");

        boost::asio::io_context context;
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), cmd_args.port);
        boost::asio::ip::tcp::acceptor acceptor(context, endpoint);

        start_accept(context, acceptor);

        Logger::info("Server stopped");
    }
    catch (const std::exception& ex)
    {
        Logger::error(ex.what());
    }
}

void Server::start_accept(boost::asio::io_context& context, boost::asio::ip::tcp::acceptor& acceptor)
{
    size_t client_id = 0;

    while (true)
    {
        // create a socket per a client and wait for a connection
        boost::asio::ip::tcp::socket socket(context);
        acceptor.accept(socket);

        ++client_id;

        TcpClient tcp_client(std::move(socket));
        ServerSession session(std::move(tcp_client), client_id);

        session.start();
    }
}
