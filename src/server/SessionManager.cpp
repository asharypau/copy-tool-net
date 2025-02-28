#include "SessionManager.h"

#include <format>

using namespace Server;

SessionManager::SessionManager(boost::asio::io_context& context)
    : _context(context),
      _sessions()
{
}

void SessionManager::start_new(const size_t client_id, boost::asio::ip::tcp::socket socket)
{
    const auto& [it, emplaced] = _sessions.try_emplace(client_id, std::make_unique<Session>(client_id, std::move(socket)));
    if (emplaced)
    {
        boost::asio::co_spawn(
            _context,
            _sessions[client_id]->run(
                [this](const size_t id)
                {
                    end_session(id);
                }),
            boost::asio::detached);
    }
    else
    {
        Logger::error(std::format("Client {} already exists", client_id));
    }
}

void SessionManager::end_session(const size_t client_id)
{
    const std::unordered_map<size_t, std::unique_ptr<Session>>::iterator& it = _sessions.find(client_id);
    if (it != _sessions.end())
    {
        _sessions.erase(client_id);
    }
    else
    {
        Logger::error(std::format("Unable to terminate session, because invalid client id was provided: {}", client_id));
    }
}
