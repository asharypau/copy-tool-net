#ifndef IAPPLICATION_H
#define IAPPLICATION_H

#include "../models/CmdArgs.h"

class IApplication
{
public:
    /**
     * @brief Executes the application with the given command line arguments.
     *
     * @param cmd_args The command line arguments to be processed by the application.
     */
    virtual void run(CmdArgs cmd_args) = 0;

    virtual ~IApplication() = default;
};

#endif  // IAPPLICATION_H
