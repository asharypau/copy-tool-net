#include "Session.h"

#include <exception>
#include <format>

#include "../common/network/tcp/OperationException.h"

using namespace Server;

Session::Session(const size_t client_id, Tcp::Socket socket)
    : _socket(std::move(socket)),
      _tcp_reader(_socket),
      _tcp_writer(_socket),
      _file_storage(std::to_string(client_id)),
      _dispatcher(_tcp_reader, _tcp_writer, _file_storage),
      _client_id(client_id)
{
}

boost::asio::awaitable<bool> Session::handshake()
{
    try
    {
        co_await _socket.handshake(Tcp::HandshakeType::SERVER);

        co_return true;
    }
    catch (const std::exception& ex)
    {
        Logger::error(std::format("An error occurred during session handshake: {} for the client {}", ex.what(), _client_id));
        stop();

        co_return false;
    }
}

boost::asio::awaitable<void> Session::run_request_processing_loop()
{
    while (_socket.is_open())
    {
        try
        {
            auto [request_metadata, _] = co_await _tcp_reader.read_async<RequestMetadata>();
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
            Logger::error(std::format("An error occurred during the session run: {} for the client {}", ex.what(), _client_id));

            if (ex.error_code() == boost::asio::error::eof || ex.error_code() == boost::asio::error::connection_reset)
            {
                stop();
            }
        }
        catch (const std::exception& ex)
        {
            Logger::error(std::format("An error occurred during the session run: {} for the client {}", ex.what(), _client_id));

            stop();
        }
    }
}

void Session::stop()
{
    try
    {
        _socket.close();
        Logger::info(std::format("Session stopped for the client {}", _client_id));
    }
    catch (const std::exception& ex)
    {
        Logger::error(std::format("An error occurred during session stop: {} for the client {}", ex.what(), _client_id));
    }
}
