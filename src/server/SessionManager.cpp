#include "SessionManager.h"

#include <format>

#include "../utils/Logger.h"

using namespace Server;

SessionManager::SessionManager(boost::asio::io_context& context)
    : _context(context),
      _sessions()
{
}

void SessionManager::start_new(const std::size_t client_id, Tcp::Socket socket)
{
    if (!_sessions.contains(client_id))
    {
        _sessions.emplace(client_id, std::make_unique<Session>(client_id, std::move(socket)));
        boost::asio::co_spawn(
            _context,
            _sessions[client_id]->run(
                [this](const std::size_t id)
                {
                    end_session(id);
                }),
            boost::asio::detached);
    }
    else
    {
        Logger::error(std::format("Session has already started for the client {}", client_id));
    }
}

void SessionManager::end_session(const std::size_t client_id)
{
    if (_sessions.empty())
    {
        Logger::error(std::format("Unable to terminate the session, because there are no active sessions for client {}", client_id));
        return;
    }

    const std::unordered_map<std::size_t, std::unique_ptr<Session>>::iterator& it = _sessions.find(client_id);
    if (it != _sessions.end())
    {
        _sessions.erase(client_id);
    }
    else
    {
        Logger::error(std::format("Unable to terminate the session, because invalid client id was provided: {}", client_id));
    }
}
