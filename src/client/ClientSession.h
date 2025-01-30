#ifndef CLIENT_SESSION_H
#define CLIENT_SESSION_H

#include "../common/TcpClient.h"
#include "FilePathPrompt.h"

class ClientSession
{
public:
    explicit ClientSession(TcpClient tcp_client);

    void start();

private:
    void write();
    void write_file(const std::string& file_name);

    TcpClient _tcp_client;
    FilePathPrompt _file_path_prompt;
};

#endif  // CLIENT_SESSION_H