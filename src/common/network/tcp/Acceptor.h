#ifndef TCP_ACCEPTOR_H
#define TCP_ACCEPTOR_H

#include <boost/asio.hpp>

#include "OperationResult.h"

namespace Tcp
{
    class Acceptor
    {
    public:
        Acceptor(unsigned short port, boost::asio::io_context& context);

        boost::asio::awaitable<Tcp::OperationResult<boost::asio::ip::tcp::socket>> accept();

    private:
        boost::asio::ip::tcp::acceptor _acceptor;
    };
}

#endif  // TCP_ACCEPTOR_H
