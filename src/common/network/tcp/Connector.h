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
        static boost::asio::awaitable<Tcp::OperationResult<bool>> connect(unsigned short port, const std::string& host, boost::asio::ip::tcp::socket& socket);
    };
}

#endif  // TCP_CONNECTOR_H
