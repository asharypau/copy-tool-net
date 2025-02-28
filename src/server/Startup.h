#ifndef SERVER_STARTUP_H
#define SERVER_STARTUP_H

#include <boost/asio.hpp>
#include <cstddef>

#include "../common/network/tcp/Acceptor.h"
#include "../infrastructure/IApplication.h"
#include "SessionManager.h"

namespace Server
{
    class Startup : public IApplication
    {
    public:
        explicit Startup(unsigned short port);

        void run() override;

    private:
        /**
         * @brief Accepts incoming connections.
         *
         * This method initiates an asynchronous accept operation to wait for a new
         * connection. When a new connection is accepted, it creates a new session
         * for the client and starts it.
         */
        boost::asio::awaitable<void> accept();

        boost::asio::io_context _context;
        Tcp::Acceptor _acceptor;
        SessionManager _session_manager;
        size_t _client_id;
    };
}

#endif  // SERVER_STARTUP_H
