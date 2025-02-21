#ifndef TCP_ACCEPTOR_H
#define TCP_ACCEPTOR_H

#include <boost/asio.hpp>
#include <string>

#include "../../../utils/Logger.h"
#include "Utils.h"

namespace Tcp
{
    class Acceptor
    {
    public:
        Acceptor(unsigned short port, boost::asio::io_context& context)
            : _acceptor(context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
        {
        }

        /**
         * @brief Initiates an asynchronous TCP connection acceptance.
         *
         * The function operates as follows:
         * - Calls `async_accept` on the `_acceptor` object to wait for an incoming client connection.
         * - If the connection attempt is successful, the provided callback is invoked with the accepted socket, allowing the caller to handle the communication.
         * - If an error occurs, the error is passed to an error handler.
         * - Catches any exceptions thrown during the process and logs them..
         *
         * @tparam TCallback The type of the callback function to be invoked upon completion.
         *
         * @param callback The callback function to be executed once the operation completes.
         */
        template <class TCallback>
        void accept(TCallback&& callback)
        {
            _acceptor.async_accept(
                [this, callback = std::forward<TCallback>(callback)](const boost::system::error_code& error, boost::asio::ip::tcp::socket socket)
                {
                    try
                    {
                        if (error)
                        {
                            Tcp::Utils::handle_error(error);
                        }
                        else
                        {
                            callback(std::move(socket));
                        }
                    }
                    catch (const std::exception& ex)
                    {
                        Logger::error("Error occurred during accept: " + std::string(ex.what()));
                    }
                });
        }

        boost::asio::awaitable<boost::asio::ip::tcp::socket> accept()
        {
            co_return co_await _acceptor.async_accept(boost::asio::use_awaitable);
        }

    private:
        boost::asio::ip::tcp::acceptor _acceptor;
    };
}

#endif  // TCP_ACCEPTOR_H
