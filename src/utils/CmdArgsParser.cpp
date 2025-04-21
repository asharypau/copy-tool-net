#include "CmdArgsParser.h"

#include <string>

CmdArgs CmdArgsParser::parse(int argc, char* argv[])
{
    CmdArgs cmd_args;

    for (std::size_t i = 1; i < argc; i++)
    {
        if (cmd_args.port == 0 && exists(argv[i], port_tokens))
        {
            cmd_args.port = get<unsigned short>(argv[i]);
        }

        if (cmd_args.host.empty() && exists(argv[i], host_tokens))
        {
            cmd_args.host = get<std::string>(argv[i]);
        }

        if (cmd_args.app_type == ApplicationType::UNKNOWN && exists(argv[i], app_type_tokens))
        {
            cmd_args.app_type = get<ApplicationType>(argv[i]);
        }
    }

    return cmd_args;
}
