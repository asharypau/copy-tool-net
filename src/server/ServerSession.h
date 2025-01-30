#ifndef SERVER_SESSION_H
#define SERVER_SESSION_H

#include "../common/TcpClient.h"

class ServerSession
{
public:
    ServerSession(TcpClient tcp_client, size_t client_id);

    void start();

private:
    void read();
    size_t read_size();
    std::vector<char> read_batch(size_t batch_size);

    TcpClient _tcp_client;
    size_t _client_id;
};

#endif  // SERVER_SESSION_H
