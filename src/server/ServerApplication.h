#ifndef SERVER_APPLICATION_H
#define SERVER_APPLICATION_H

#include <boost/asio.hpp>

#include "../common/IApplication.h"

class ServerApplication : public IApplication
{
public:
    void run(CmdArgs cmd_args) override;

private:
    void read(boost::asio::ip::tcp::socket& socket);
};

#endif  // SERVER_APPLICATION_H
