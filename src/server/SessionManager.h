#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include <boost/asio.hpp>

#include "Session.h"

namespace Server
{
    class SessionManager
    {
    public:
        explicit SessionManager(boost::asio::io_context& context);

        void start_new(size_t client_id, boost::asio::ip::tcp::socket socket);

    private:
        void end_session(const std::string& client_id);

        boost::asio::io_context& _context;
        std::unordered_map<std::string, std::unique_ptr<Session>> _sessions;
    };
}

#endif  // SESSION_MANAGER_H
