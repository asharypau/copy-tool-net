#ifndef TCP_ACCEPTOR_H
#define TCP_ACCEPTOR_H

#include <boost/asio.hpp>
#include <utility>

#include "OperationResult.h"
#include "boost/asio/use_awaitable.hpp"

namespace Tcp
{
    class Acceptor
    {
    public:
        Acceptor(unsigned short port, boost::asio::io_context& context)
            : _acceptor(context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
        {
        }

        boost::asio::awaitable<Tcp::OperationResult<boost::asio::ip::tcp::socket>> accept()
        {
            auto [error, socket] = co_await _acceptor.async_accept(_acceptor.get_executor(), boost::asio::as_tuple(boost::asio::use_awaitable));

            co_return Tcp::OperationResult<boost::asio::ip::tcp::socket>(std::move(socket), error);
        }

    private:
        boost::asio::ip::tcp::acceptor _acceptor;
    };
}

#endif  // TCP_ACCEPTOR_H
