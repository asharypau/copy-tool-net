#ifndef SESSION_H
#define SESSION_H

#include <boost/asio.hpp>
#include <memory>
#include <string>

#include "../common/network/tcp/Reader.h"
#include "../common/network/tcp/Writer.h"
#include "Dispatcher.h"

namespace Server
{
    class Session : public std::enable_shared_from_this<Session>
    {
    public:
        Session(std::string client_id, boost::asio::ip::tcp::socket socket)
            : _client_id(client_id),
              _socket(std::move(socket)),
              _tcp_reader(_socket),
              _tcp_writer(_socket),
              _dispatcher(_client_id, _tcp_reader, _tcp_writer)
        {
            Logger::info(std::format("Client {} connected", _client_id));
        }

        ~Session()
        {
            Logger::info(std::format("Client {} disconnected", _client_id));

            _socket.close();
        }

        /**
         * @brief Runs a new session.
         */
        template <class TCallback>
        boost::asio::awaitable<void> run(TCallback&& callback)
        {
            while (_socket.is_open())
            {
                try
                {
                    RequestMetadata request_metadata = co_await _tcp_reader.read_async<RequestMetadata>();
                    co_await _dispatcher.handle(request_metadata);
                }
                catch (const boost::system::system_error& ex)
                {
                    Logger::error(std::format("An error occurred during session run: {}", ex.what()));

                    if (ex.code() == boost::asio::error::eof || ex.code() == boost::asio::error::connection_reset)
                    {
                        break;
                    }
                }
                catch (const std::exception& ex)
                {
                    Logger::error(std::format("Unknown error occurred during session run: {}", ex.what()));

                    break;
                }
            }

            callback(_client_id);
        }

    private:
        std::string _client_id;
        boost::asio::ip::tcp::socket _socket;
        Tcp::Reader _tcp_reader;
        Tcp::Writer _tcp_writer;
        Dispatcher _dispatcher;
    };
}

#endif  // SESSION_H
