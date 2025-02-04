#include "FilePathPrompt.h"

#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>

#include "../models/Colors.h"

std::vector<std::pair<std::string, std::string>> FilePathPrompt::get()
{
    std::cout << std::endl
              << Colors::WHITE
              << "====================================================================================================================================================="
              << Colors::RESET << std::endl;

    std::vector<std::pair<std::string, std::string>> result = internal_get();
    if (result.empty())
    {
        std::cout << Colors::RED << "No files to send. " << Colors::RESET << "Do you want to try again? (Y/N): ";

        char choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 'y' || choice == 'Y')
        {
            result = internal_get();
        }
    }
    else
    {
        std::cout << Colors::GREEN << "Files to send: " << Colors::RESET << std::endl;

        for (int i = 0; i < result.size(); ++i)
        {
            std::cout << i + 1 << ". " << result[i].first << ':' << result[i].second << std::endl;
        }
    }

    std::cout << Colors::WHITE
              << "====================================================================================================================================================="
              << Colors::RESET
              << std::endl;

    return std::move(result);
}

std::vector<std::pair<std::string, std::string>> FilePathPrompt::internal_get()
{
    std::cout << Colors::WHITE << "Enter one or more files to send. Input format " << Colors::YELLOW << "filename:filepath: " << Colors::RESET;

    std::string input;
    std::getline(std::cin, input);

    std::istringstream ss(input);
    std::vector<std::pair<std::string, std::string>> files_data;
    std::string file_data;

    while (ss >> file_data)
    {
        size_t delimiter_pos = file_data.find(':');
        if (delimiter_pos == std::string::npos)
        {
            std::cout << Colors::RED << "Incorrect format: " << Colors::RESET << file_data << std::endl;
            continue;
        }

        std::string name = file_data.substr(0, delimiter_pos);
        std::string path = file_data.substr(delimiter_pos + 1, file_data.size());

        if (std::filesystem::exists(path))
        {
            files_data.emplace_back(name, path);
        }
        else
        {
            std::cout << Colors::RED << "File does not exist: " << Colors::RESET << file_data << std::endl;
        }
    }

    return std::move(files_data);
}
