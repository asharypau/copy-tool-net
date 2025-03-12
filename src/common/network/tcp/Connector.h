#ifndef TCP_CONNECTOR_H
#define TCP_CONNECTOR_H

#include <boost/asio.hpp>
#include <format>
#include <string>

#include "../../../utils/Logger.h"
#include "OperationResult.h"

namespace Tcp
{
    class Connector
    {
    public:
        static boost::asio::awaitable<Tcp::OperationResult<bool>> connect(unsigned short port, std::string host, boost::asio::ip::tcp::socket& socket)
        {
            boost::asio::ip::tcp::resolver resolver(socket.get_executor());
            boost::asio::ip::tcp::resolver::results_type endpoint = co_await resolver.async_resolve(host, std::to_string(port), boost::asio::use_awaitable);
            if (endpoint.empty())
            {
                co_return Tcp::OperationResult<bool>(false, -1, std::format("Failed to resolve endpoint by provided {}: {}", host, port));
            }

            auto [error, end] = co_await boost::asio::async_connect(socket, endpoint, boost::asio::as_tuple(boost::asio::use_awaitable));
            if (!error)
            {
                Logger::info("Connected by " + host + ":" + std::to_string(port));
            }

            co_return Tcp::OperationResult<bool>(!!error, error);
        }
    };
}

#endif  // TCP_CONNECTOR_H
