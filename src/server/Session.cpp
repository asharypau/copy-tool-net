#include "Session.h"

#include <memory>
#include <string>
#include <utility>

using namespace Server;

Session::Session(boost::asio::ip::tcp::socket socket, size_t client_id)
    : _socket(std::move(socket)),
      _message_reader(_socket),
      _tcp_writer(_socket),
      _client_id(std::to_string(client_id))
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
    std::shared_ptr<Session> self = shared_from_this();  // to extend the lifetime

    _message_reader.read(
        _client_id,
        [this, self](size_t file_id)
        {
            std::shared_ptr<size_t> id = std::make_shared<size_t>(file_id);
            _tcp_writer.write(
                id.get(),
                Tcp::HEADER_SIZE, [id] {});  // capture the file_id to to extend the lifetime

            run();
        });
}
