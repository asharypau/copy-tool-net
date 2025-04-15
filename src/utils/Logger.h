#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>

#include "Colors.h"

class Logger
{
public:
    static void debug(const std::string& message)
    {
        std::cout << Colors::WHITE << "[DEBUG] " << Colors::RESET << message << std::endl;
    }

    static void info(const std::string& message)
    {
        std::cout << Colors::WHITE << "[INFO] " << Colors::RESET << message << std::endl;
    }

    static void warning(const std::string& message)
    {
        std::cout << Colors::YELLOW << "[WARNING] " << Colors::RESET << message << std::endl;
    }

    static void error(const std::string& message)
    {
        std::cout << Colors::RED << "[ERROR] " << Colors::RESET << message << std::endl;
    }
};

#endif
