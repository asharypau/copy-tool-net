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

        /**
         * @brief Starts a new session for a given client.
         *
         * This function initializes and starts a new session using the provided client id and TCP socket.
         *
         * @param client_id The unique identifier of the client.
         * @param socket The TCP socket associated with the client.
         */
        void run_new(const std::size_t client_id, Tcp::Socket socket);

        std::size_t get_count() const noexcept
        {
            return _sessions.size();
        }

    private:
        /**
         * @brief Ends a session for a given client.
         *
         * @param client_id The unique identifier of the client whose session should be terminated.
         */
        void end_session(const std::size_t client_id);

        boost::asio::io_context& _context;
        std::unordered_map<std::size_t, std::unique_ptr<Session>> _sessions;
    };
}

#endif  // SESSION_MANAGER_H
