#ifndef APPLICATION_BUILDER_H
#define APPLICATION_BUILDER_H

#include <memory>

#include "../common/IApplication.h"
#include "../models/CmdArgs.h"

class ApplicationBuilder
{
public:
    /**
     * @brief Builds an application of the specified type.
     *
     * @param type The type of application to build.
     * @return std::unique_ptr<IApplication> A unique pointer to the built application.
     */
    std::unique_ptr<IApplication> build(CmdArgs args);
};

#endif  // APPLICATION_BUILDER_H
