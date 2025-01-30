#include "FilePathPrompt.h"

#include <filesystem>
#include <iostream>
#include <sstream>

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"

std::vector<std::string> FilePathPrompt::get()
{
    std::vector<std::string> result;

    std::cout << "Enter one or more files to send: ";

    std::string input;
    std::getline(std::cin, input);
    std::istringstream file_names(input);

    std::string file_name;
    while (file_names >> file_name)
    {
        if (exists(file_name))
        {
            result.push_back(file_name);
        }
        else
        {
            std::cout << RED << "File does not exist: " << RESET << file_name << std::endl;
        }
    }

    return std::move(result);
}

bool FilePathPrompt::exists(const std::string& file_name)
{
    return std::filesystem::exists(file_name);
}
