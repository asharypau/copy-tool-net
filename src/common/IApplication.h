#ifndef IAPPLICATION_H
#define IAPPLICATION_H

#include "CmdArgs.h"

class IApplication
{
public:
    virtual void run(CmdArgs cmd_args) = 0;

    virtual ~IApplication() = default;
};

#endif  // IAPPLICATION_H
