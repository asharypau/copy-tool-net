#include "ApplicationBuilder.h"

#include <stdexcept>

#include "../client/Client.h"
#include "../server/Server.h"

std::unique_ptr<IApplication> ApplicationBuilder::build(CmdArgs args)
{
    switch (args.app_type)
    {
    case ApplicationType::SERVER:
        return std::make_unique<Server>(args.port);
    case ApplicationType::CLIENT:
        return std::make_unique<Client>(args.port, args.host);
    default:
        throw std::invalid_argument("Invalid application type");
    }
}
