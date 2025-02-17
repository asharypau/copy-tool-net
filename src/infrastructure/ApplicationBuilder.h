#ifndef APPLICATION_BUILDER_H
#define APPLICATION_BUILDER_H

#include <memory>

#include "../utils/CmdArgs.h"
#include "IApplication.h"

class ApplicationBuilder
{
public:
    /**
     * @brief Builds an application of the specified type.
     *
     * @param type The type of application to build.
     * @return std::unique_ptr<IApplication> A unique pointer to the built application.
     */
    std::unique_ptr<IApplication> build(CmdArgs args) const;
};

#endif  // APPLICATION_BUILDER_H
