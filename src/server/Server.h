#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>

#include "../common/IApplication.h"

class Server : public IApplication
{
public:
    void run(CmdArgs cmd_args) override;

private:
    void start_accept(boost::asio::io_context& context, boost::asio::ip::tcp::acceptor& acceptor);
};

#endif  // SERVER_H
