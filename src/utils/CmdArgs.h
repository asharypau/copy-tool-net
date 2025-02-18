#ifndef CMD_ARGS_H
#define CMD_ARGS_H

#include <string>

#include "../infrastructure/ApplicationType.h"

struct CmdArgs
{
    unsigned short port = 0;
    std::string host;
    ApplicationType app_type = ApplicationType::UNKNOWN;
};

#endif  // CMD_ARGS_H
