#ifndef TCP_CONNECTOR_H
#define TCP_CONNECTOR_H

#include <boost/asio.hpp>
#include <string>

#include "OperationResult.h"

namespace Tcp
{
    class Connector
    {
    public:
        /**
         * @brief Asynchronously connects to a remote host on a specified port.
         *
         * @param port The port number to which the socket should connect.
         * @param host The hostname or IP address of the remote server.
         * @param socket A reference to the `boost::asio::ip::tcp::socket` to be used for the connection.
         *
         * @return boost::asio::awaitable<Tcp::OperationResult<bool>> An awaitable coroutine that returns
         * an `OperationResult` containing `true` if the connection was successful, or `false` if it failed.
         */
        static boost::asio::awaitable<Tcp::OperationResult<bool>> connect(unsigned short port, const std::string& host, boost::asio::ip::tcp::socket& socket);
    };
}

#endif  // TCP_CONNECTOR_H
