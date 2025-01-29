#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>

class Logger
{
public:
    static void info(const std::string& message)
    {
        std::cout << "[INFO]: " << message << std::endl;
    }

    static void warning(const std::string& message)
    {
        std::cout << "[WARNING]: " << message << std::endl;
    }

    static void error(const std::string& message)
    {
        std::cout << "[ERROR]: " << message << std::endl;
    }
};

#endif
