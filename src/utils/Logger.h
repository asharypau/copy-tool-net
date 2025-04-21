#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>

#include "Colors.h"
#include "DateTimeProvider.h"

class Logger
{
public:
    static void debug(const std::string& message)
    {
        std::cout << Colors::BRIGHT_BLUE << "[DEBUG]\t" << Colors::WHITE << DateTimeProvider::utc_now() << '\t' << message << Colors::RESET << std::endl;
    }

    static void info(const std::string& message)
    {
        std::cout << Colors::GREEN << "[INFO]\t" << Colors::WHITE << DateTimeProvider::utc_now() << '\t' << message << Colors::RESET << std::endl;
    }

    static void warning(const std::string& message)
    {
        std::cout << Colors::YELLOW << "[WARN]\t" << Colors::WHITE << DateTimeProvider::utc_now() << '\t' << message << Colors::RESET << std::endl;
    }

    static void error(const std::string& message)
    {
        std::cout << Colors::RED << "[ERROR]\t" << Colors::WHITE << DateTimeProvider::utc_now() << '\t' << message << Colors::RESET << std::endl;
    }
};

#endif
