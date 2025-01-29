#ifndef CLIENT_APPLICATION_H
#define CLIENT_APPLICATION_H

#include <boost/asio.hpp>
#include <vector>

#include "../common/IApplication.h"

class ClientApplication : public IApplication
{
public:
    void run(CmdArgs cmd_args) override;

private:
    void write(boost::asio::ip::tcp::socket& socket);
    void write_batch(boost::asio::ip::tcp::socket& socket, const std::string& file_name);
    std::vector<std::string> get_file_names();
};

#endif  // CLIENT_APPLICATION_H
