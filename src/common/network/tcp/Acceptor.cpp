#include "Acceptor.h"

#include <utility>

using namespace Tcp;

Acceptor::Acceptor(unsigned short port, boost::asio::io_context& context)
    : _acceptor(context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
{
}

boost::asio::awaitable<Tcp::OperationResult<boost::asio::ip::tcp::socket>> Acceptor::accept()
{
    auto [error, socket] = co_await _acceptor.async_accept(_acceptor.get_executor(), boost::asio::as_tuple(boost::asio::use_awaitable));

    co_return Tcp::OperationResult<boost::asio::ip::tcp::socket>(std::move(socket), error);
}
