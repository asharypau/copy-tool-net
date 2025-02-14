#include "Session.h"

#include <string>

#include "../models/Endpoints.h"

using namespace Server;

Session::Session(size_t client_id, boost::asio::ip::tcp::socket socket)
    : _client_id(std::to_string(client_id)),
      _socket(std::move(socket)),
      _tcp_reader(_socket),
      _tcp_writer(_socket),
      _dispatcher(_client_id, _tcp_reader, _tcp_writer),
      _endpoint(Endpoints::LENGTH, '\0')
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

    _tcp_reader.read_async(
        Endpoints::LENGTH + Tcp::HEADER_SIZE,
        [this, self]
        {
            _tcp_reader.extract(_endpoint.data(), Endpoints::LENGTH);  // get endpoint

            size_t request_size = 0;
            _tcp_reader.extract(&request_size, Tcp::HEADER_SIZE);  // get request size

            _dispatcher.handle(request_size, std::string(_endpoint.data()), self);
        });
}

void Session::notify_done()
{
    run();
}
