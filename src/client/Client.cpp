#include "Client.h"

#include <boost/asio.hpp>
#include <string>

#include "../common/TcpClient.h"
#include "../utils/Logger.h"
#include "ClientSession.h"

void Client::run(CmdArgs cmd_args)
{
    try
    {
        Logger::info("Client started");

        boost::asio::io_context context;
        boost::asio::ip::tcp::resolver resolver(context);
        boost::asio::ip::tcp::socket socket(context);

        boost::asio::ip::tcp::resolver::results_type endpoint = resolver.resolve(cmd_args.host, std::to_string(cmd_args.port));

        connect(socket, endpoint);

        TcpClient tcp_client(std::move(socket));
        ClientSession session(std::move(tcp_client));

        session.start();
    }
    catch (const std::exception& ex)
    {
        Logger::error(ex.what());
    }
}
