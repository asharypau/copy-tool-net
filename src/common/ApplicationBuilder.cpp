#include "ApplicationBuilder.h"

#include <stdexcept>

#include "../client/Client.h"
#include "../server/Server.h"

std::unique_ptr<IApplication> ApplicationBuilder::build(ApplicationType type)
{
    switch (type)
    {
    case ApplicationType::SERVER:
        return std::make_unique<Server>();
    case ApplicationType::CLIENT:
        return std::make_unique<Client>();
    default:
        throw std::invalid_argument("Invalid application type");
    }
}
