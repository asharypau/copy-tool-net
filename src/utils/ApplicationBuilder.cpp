#include "ApplicationBuilder.h"

#include <memory>
#include <stdexcept>

#include "../client/ClientApplication.h"
#include "../server/ServerApplication.h"

std::unique_ptr<IApplication> ApplicationBuilder::build(ApplicationType type)
{
    switch (type)
    {
    case ApplicationType::SERVER:
        return std::make_unique<ServerApplication>();
    case ApplicationType::CLIENT:
        return std::make_unique<ClientApplication>();
    default:
        throw std::invalid_argument("Invalid application type");
    }
}
