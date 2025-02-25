#include "SessionManager.h"

#include <format>
#include <string>

using namespace Server;

SessionManager::SessionManager(boost::asio::io_context& context)
    : _context(context),
      _sessions()
{
}

void SessionManager::start_new(size_t client_id, boost::asio::ip::tcp::socket socket)
{
    std::string client_id_str = std::to_string(client_id);

    _sessions.emplace(client_id_str, std::make_unique<Session>(client_id_str, std::move(socket)));
    boost::asio::co_spawn(
        _context,
        _sessions[client_id_str]->run(
            [this](std::string& client_id)
            {
                end_session(client_id);
            }),
        boost::asio::detached);
}

void SessionManager::end_session(const std::string& client_id)
{
    const std::unordered_map<std::string, std::unique_ptr<Session>>::iterator& it = _sessions.find(client_id);
    if (it != _sessions.end())
    {
        _sessions.erase(client_id);
    }
    else
    {
        Logger::error(std::format("Invalid client id: {}", client_id));
    }
}
