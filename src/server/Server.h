#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>

#include "../common/IApplication.h"

class Server : public IApplication
{
public:
    void run(CmdArgs cmd_args) override;

private:
    void start_accept(const std::string& host, unsigned short port);
};

#endif  // SERVER_H
