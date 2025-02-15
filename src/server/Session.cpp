#include "Session.h"

#include "../models/RequestMetadata.h"

using namespace Server;

Session::Session(size_t client_id, boost::asio::ip::tcp::socket socket)
    : _client_id(std::to_string(client_id)),
      _socket(std::move(socket)),
      _tcp_reader(_socket),
      _tcp_writer(_socket),
      _dispatcher(_client_id, _tcp_reader, _tcp_writer)
{
    Logger::info("Client connected: " + _client_id);
}

Session::~Session()
{
    Logger::info("Client disconnected: " + _client_id);

    _socket.close();
}

void Session::run()
{
    std::shared_ptr<Session> self = shared_from_this();  // to extend the session lifetime

    _tcp_reader.read_async<RequestMetadata>(
        [this, self](RequestMetadata request_metadata)
        {
            _dispatcher.handle(request_metadata, self);
        });
}

void Session::notify_done()
{
    run();
}
