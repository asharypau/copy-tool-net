#ifndef SESSION_H
#define SESSION_H

#include <boost/asio.hpp>
#include <exception>
#include <format>

#include "../common/network/tcp/Reader.h"
#include "../common/network/tcp/Writer.h"
#include "../utils/Logger.h"
#include "Dispatcher.h"
#include "services/FileStorage.h"

namespace Server
{
    class Session
    {
    public:
        Session(const size_t client_id, boost::asio::ip::tcp::socket socket)
            : _socket(std::move(socket)),
              _tcp_reader(_socket),
              _tcp_writer(_socket),
              _file_storage(std::to_string(client_id)),
              _dispatcher(_tcp_reader, _tcp_writer, _file_storage),
              _client_id(client_id)
        {
        }

        ~Session()
        {
            try
            {
                _socket.close();
            }
            catch (const std::exception& ex)
            {
                Logger::error(std::format("An error occurred during session destruction: {} for the client {}", ex.what(), _client_id));
            }
            catch (...)
            {
                Logger::error(std::format("An unknown error occurred during session destruction for the client {}", _client_id));
            }
        }

        /**
         * @brief Runs a new session.
         */
        template <class TCallback>
        boost::asio::awaitable<void> run(TCallback callback)
        {
            while (true)
            {
                try
                {
                    RequestMetadata request_metadata = co_await _tcp_reader.read_async<RequestMetadata>();
                    if (request_metadata.size != 0 && !request_metadata.endpoint.empty())
                    {
                        co_await _dispatcher.handle(request_metadata);
                    }
                    else
                    {
                        Logger::warning(std::format("Invalid RequestMetadata was received for the client {}", _client_id));
                    }
                }
                catch (const Tcp::OperationException& ex)
                {
                    // int error_code = ex.error_code();
                    // std::string error_message = ex.what();
                    Logger::error(std::format("An error occurred during session run: {} for the client {}", ex.what(), _client_id));

                    if (ex.error_code() == boost::asio::error::eof || ex.error_code() == boost::asio::error::connection_reset)
                    {
                        break;
                    }
                }
                catch (const std::exception& ex)
                {
                    Logger::error(std::format("An error error occurred during session run: {} for the client {}", ex.what(), _client_id));

                    break;
                }
                catch (...)
                {
                    Logger::error(std::format("An unknown error occurred during session run for the client {}", _client_id));

                    break;
                }
            }

            callback(_client_id);
        }

    private:
        boost::asio::ip::tcp::socket _socket;
        Tcp::Reader _tcp_reader;
        Tcp::Writer _tcp_writer;
        FileStorage _file_storage;
        Dispatcher _dispatcher;
        const size_t _client_id;
    };
}

#endif  // SESSION_H
