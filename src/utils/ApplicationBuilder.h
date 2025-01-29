#ifndef APPLICATION_BUILDER_H
#define APPLICATION_BUILDER_H

#include <memory>

#include "../common/ApplicationType.h"
#include "../common/IApplication.h"

class ApplicationBuilder
{
public:
    std::unique_ptr<IApplication> build(ApplicationType type);
};

#endif  // APPLICATION_BUILDER_H
