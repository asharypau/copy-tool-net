#include "Server.h"

#include "../utils/Logger.h"
#include "Session.h"

void Server::run(CmdArgs cmd_args)
{
    try
    {
        Logger::info("Server started");

        start_accept(cmd_args.host, cmd_args.port);

        Logger::info("Server stopped");
    }
    catch (const std::exception& ex)
    {
        Logger::error(ex.what());
    }
}

void Server::start_accept(const std::string& host, unsigned short port)
{
    size_t client_id = 0;
    boost::asio::io_context context;

    while (true)
    {
        ++client_id;

        TcpClient tcp_client(context);
        tcp_client.accept(host, port);

        Session session(std::move(tcp_client), client_id);
        session.start();
    }
}
