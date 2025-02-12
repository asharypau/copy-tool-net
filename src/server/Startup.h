#ifndef SERVER_STARTUP_H
#define SERVER_STARTUP_H

#include <boost/asio.hpp>

#include "../common/IApplication.h"
#include "../common/Tcp.h"

namespace Server
{
    static size_t CLIENT_ID = 0;
    static constexpr std::string_view CLIENT_STORAGE_PATH = "D:/Education/CppMentoring/files/output/";

    class Startup : public IApplication
    {
    public:
        explicit Startup(unsigned short port);

        void run() override;

    private:
        void create_client_storage();

        /**
         * @brief Accepts incoming connections.
         *
         * This method initiates an asynchronous accept operation to wait for a new
         * connection. When a new connection is accepted, it creates a new session
         * for the client and starts it.
         */
        void accept();

        boost::asio::io_context _context;
        Tcp::Acceptor _acceptor;
    };
}

#endif  // SERVER_STARTUP_H
