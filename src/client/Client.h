#ifndef CLIENT_H
#define CLIENT_H

#include "../common/IApplication.h"
#include "../common/TcpClient.h"
#include "FilePathPrompt.h"
#include "FileReader.h"

class Client : public IApplication
{
public:
    Client();

    void run(CmdArgs cmd_args) override;

private:
    void write(TcpClient& tcp_client);
    void write_file(TcpClient& tcp_client, FileReader& file);

    FilePathPrompt _file_path_prompt;
};

#endif  // CLIENT_H
