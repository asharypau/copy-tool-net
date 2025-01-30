#ifndef CLIENT_H
#define CLIENT_H

#include "../common/IApplication.h"

class Client : public IApplication
{
public:
    void run(CmdArgs cmd_args) override;
};

#endif  // CLIENT_H
