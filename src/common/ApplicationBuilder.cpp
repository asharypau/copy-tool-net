#include "ApplicationBuilder.h"

#include <stdexcept>

#include "../client/Startup.h"
#include "../server/Startup.h"

std::unique_ptr<IApplication> ApplicationBuilder::build(CmdArgs args)
{
    switch (args.app_type)
    {
    case ApplicationType::SERVER:
        return std::make_unique<Server::Startup>(args.port);
    case ApplicationType::CLIENT:
        return std::make_unique<Client::Startup>(args.port, args.host);
    default:
        throw std::invalid_argument("Invalid application type");
    }
}
