#ifndef CLIENT_H
#define CLIENT_H

#include <string>

#include "../common/IApplication.h"
#include "FilePathPrompt.h"
#include "FileReader.h"
#include "TcpClient.h"

class Client : public IApplication
{
public:
    Client(unsigned short port, const std::string& host);

    void run() override;

private:
    void write();
    void write_file(FileReader& file);

    TcpClient _tcp_client;
    FilePathPrompt _file_path_prompt;
};

#endif  // CLIENT_H
