#include "FilePathPrompt.h"

#include <filesystem>
#include <iostream>
#include <sstream>

#include "../models/Colors.h"

std::vector<std::string> FilePathPrompt::get()
{
    std::vector<std::string> result = internal_get();

    if (result.empty())
    {
        std::cout << Colors::RED << "No files to send. " << Colors::RESET << "Do you want to try again? (Y/N): ";

        char choice;
        std::cin >> choice;
        std::cout << std::endl;

        if (choice == 'y' || choice == 'Y')
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            result = internal_get();
        }
    }
    else
    {
        std::cout << Colors::GREEN << "Files to send: " << Colors::RESET;

        for (int i = 0; i < result.size(); ++i)
        {
            std::cout << i + 1 << ". " << result[i] << std::endl;
        }
    }

    return std::move(result);
}

std::vector<std::string> FilePathPrompt::internal_get()
{
    std::cout << Colors::WHITE << "Enter one or more files to send: " << Colors::RESET;

    std::vector<std::string> result;

    std::string input;
    std::getline(std::cin, input);

    std::istringstream file_names(input);
    std::string file_name;

    while (file_names >> file_name)
    {
        if (std::filesystem::exists(file_name))
        {
            result.push_back(file_name);
        }
        else
        {
            std::cout << Colors::RED << "File does not exist: " << Colors::RESET << file_name << std::endl;
        }
    }

    return std::move(result);
}
