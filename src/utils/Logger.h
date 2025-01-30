#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>

#define RESET "\033[0m"
#define RED "\033[31m"
#define YELLOW "\033[33m"

class Logger
{
public:
    static void info(const std::string& message)
    {
        std::cout << "[INFO]: " << message << std::endl;
    }

    static void warning(const std::string& message)
    {
        std::cout << YELLOW << "[WARNING]: " << RESET << message << std::endl;
    }

    static void error(const std::string& message)
    {
        std::cout << RED << "[ERROR]: " << RESET << message << std::endl;
    }
};

#endif
