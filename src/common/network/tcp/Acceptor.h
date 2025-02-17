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
         * @tparam TCallback The type of the callback function or callable object that is executed when a connection is successfully accepted.
         *
         * @param callback The callback function or callable object to be invoked once a connection is accepted.
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

    private:
        boost::asio::ip::tcp::acceptor _acceptor;
    };
}

#endif  // TCP_ACCEPTOR_H
