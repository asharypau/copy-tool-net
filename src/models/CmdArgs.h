#ifndef CMDARGS_H
#define CMDARGS_H

#include <string>

#include "ApplicationType.h"

struct CmdArgs
{
    unsigned short port = 0;
    std::string host;
    ApplicationType app_type = ApplicationType::UNKNOWN;
};

#endif  // CMDARGS_H
