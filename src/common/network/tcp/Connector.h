#ifndef TCP_CONNECTOR_H
#define TCP_CONNECTOR_H

#include <boost/asio.hpp>
#include <string>

#include "../../../utils/Logger.h"
#include "Utils.h"

namespace Tcp
{
    class Connector
    {
    public:
        /**
         * @brief Establishes an asynchronous TCP connection to a specified host and port.
         *
         * This template function initiates an asynchronous connection to a TCP endpoint using Boost.Asio.
         * It resolves the provided host and port, and then attempts to connect to the resolved endpoint.
         * Upon a successful connection or an error, the provided callback is invoked. If the connection
         * is successful, the callback is executed. If an error occurs, it is handled and logged.
         *
         * The function operates as follows:
         * - Resolves the given host and port to obtain a list of endpoints.
         * - Attempts to asynchronously connect to the resolved endpoint using `boost::asio::async_connect`.
         * - If the connection succeeds, it calls the provided callback.
         * - If the connection fails, the error is passed to an error handler.
         * - Catches any exceptions thrown during the process and logs them.
         *
         * @tparam TCallback The type of the callback function to be invoked upon completion.
         *
         * @param port The port number to which the connection should be made.
         * @param host The host (IP address or domain) of the TCP server to connect to.
         * @param socket The `boost::asio::ip::tcp::socket` object used for the connection.
         * @param callback The callback function to be executed once the operation completes.
         */
        template <class TCallback>
        static void connect(unsigned short port, std::string host, boost::asio::ip::tcp::socket& socket, TCallback&& callback)
        {
            boost::asio::ip::tcp::resolver resolver(socket.get_executor());
            boost::asio::ip::tcp::resolver::results_type endpoint = resolver.resolve(host, std::to_string(port));
            if (endpoint.empty())
            {
                Logger::error("Failed to resolve endpoint by provided " + host + ":" + std::to_string(port));

                return;
            }

            boost::asio::async_connect(
                socket,
                endpoint,
                [host, port, callback = std::forward<TCallback>(callback)](const boost::system::error_code& error, boost::asio::ip::tcp::endpoint)
                {
                    try
                    {
                        if (error)
                        {
                            Tcp::Utils::handle_error(error);
                        }
                        else
                        {
                            Logger::info("Connected by " + host + ":" + std::to_string(port));
                        }

                        callback(error);
                    }
                    catch (const std::exception& ex)
                    {
                        Logger::error("Error occurred during connect: " + std::string(ex.what()));
                    }
                });
        }
    };
}

#endif  // TCP_CONNECTOR_H
