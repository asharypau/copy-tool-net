#ifndef CLIENT_H
#define CLIENT_H

#include <cstddef>
#include <string>

#include "../common/IApplication.h"
#include "FilePathPrompt.h"
#include "FileReader.h"
#include "TcpClient.h"

class Client : public IApplication
{
public:
    Client(unsigned short port, std::string host);

    void run() override;

private:
    void write();
    void write_file_size(size_t size);
    void write_file_name(const std::string& name);
    void write_file(FileReader& file);

    TcpClient _tcp_client;
    FilePathPrompt _file_path_prompt;
    std::string _host;
    unsigned short _port;
};

#endif  // CLIENT_H
