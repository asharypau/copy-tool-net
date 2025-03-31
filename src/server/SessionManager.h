#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include <boost/asio.hpp>

#include "../common/network/tcp/Socket.h"
#include "Session.h"

namespace Server
{
    class SessionManager
    {
    public:
        explicit SessionManager(boost::asio::io_context& context);

        void start_new(const std::size_t client_id, Tcp::Socket socket);

    private:
        void end_session(const std::size_t client_id);

        boost::asio::io_context& _context;
        std::unordered_map<std::size_t, std::unique_ptr<Session>> _sessions;
    };
}

#endif  // SESSION_MANAGER_H
