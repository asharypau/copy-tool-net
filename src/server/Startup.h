#ifndef SERVER_STARTUP_H
#define SERVER_STARTUP_H

#include <boost/asio.hpp>

#include "../common/IApplication.h"
#include "../common/Tcp.h"

namespace Server
{
    class Startup : public IApplication
    {
    public:
        explicit Startup(unsigned short port);

        void run() override;

    private:
        /**
         * @brief Asynchronously accepts incoming connections.
         *
         * This method initiates an asynchronous accept operation to wait for a new
         * connection. When a new connection is accepted, it creates a new session
         * for the client and starts it.
         */
        void accept();

        static size_t _client_id;

        boost::asio::io_context _context;
        Tcp::Acceptor _acceptor;
    };
}

#endif  // SERVER_STARTUP_H
