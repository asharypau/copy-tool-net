#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>

#include "../common/IApplication.h"

class Server : public IApplication
{
public:
    explicit Server(unsigned short port);

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
    boost::asio::ip::tcp::acceptor _acceptor;
};

#endif  // SERVER_H
