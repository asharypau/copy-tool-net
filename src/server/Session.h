#ifndef SESSION_H
#define SESSION_H

#include "../common/TcpClient.h"

class Session
{
public:
    Session(TcpClient tcp_client, size_t client_id);

    void start();

private:
    void read();
    size_t read_size();
    std::vector<char> read_batch(size_t batch_size);

    TcpClient _tcp_client;
    size_t _client_id;
};

#endif  // SESSION_H
