#include "CmdArgsParser.h"

#include <string>

CmdArgs CmdArgsParser::parse(int argc, char* argv[])
{
    CmdArgs cmd_args;

    for (size_t i = 1; i < argc; i++)
    {
        if (cmd_args.port == 0)
        {
            cmd_args.port = get<unsigned short>(argv[i], port_tokens);
        }

        if (cmd_args.host.empty())
        {
            cmd_args.host = get<std::string>(argv[i], host_tokens);
        }

        if (cmd_args.app_type == ApplicationType::UNKNOWN)
        {
            cmd_args.app_type = get<ApplicationType>(argv[i], app_type_tokens);
        }
    }

    return cmd_args;
}
