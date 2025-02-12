#include "MessagesPrompt.h"

#include <filesystem>
#include <iostream>
#include <sstream>

#include "../models/Colors.h"

using namespace Client;

std::vector<Message> MessagesPrompt::get() const
{
    std::cout << std::endl
              << Colors::WHITE
              << "====================================================================================================================================================="
              << Colors::RESET << std::endl;

    std::vector<Message> messages = internal_get();
    if (messages.empty())
    {
        std::cout << Colors::RED << "No files to send. " << Colors::RESET << "Do you want to try again? (Y/N): ";

        char choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 'y' || choice == 'Y')
        {
            messages = internal_get();
        }
    }
    else
    {
        std::cout << Colors::GREEN << "Files to send: " << Colors::RESET << std::endl;

        for (int i = 0; i < messages.size(); ++i)
        {
            std::cout << i + 1 << ". " << messages[i].name << ':' << messages[i].path << " (" + std::to_string(messages[i].size) << " bytes)" << std::endl;
        }
    }

    std::cout << Colors::WHITE
              << "====================================================================================================================================================="
              << Colors::RESET
              << std::endl;

    return std::move(messages);
}

std::vector<Message> MessagesPrompt::internal_get() const
{
    std::cout << Colors::WHITE << "Enter one or more files to send. Input format " << Colors::YELLOW << "filename:filepath: " << Colors::RESET << std::endl;

    std::string input;
    std::getline(std::cin, input);

    std::istringstream ss(input);
    std::vector<Message> messages;
    std::string message;

    while (ss >> message)
    {
        size_t delimiter_pos = message.find(':');
        if (delimiter_pos == std::string::npos)
        {
            std::cout << Colors::RED << "Incorrect format: " << Colors::RESET << message << std::endl;
            continue;
        }

        std::string name = message.substr(0, delimiter_pos);
        std::string path = message.substr(delimiter_pos + 1, message.size());

        if (std::filesystem::exists(path))
        {
            messages.emplace_back(std::filesystem::file_size(path), name, path);
        }
        else
        {
            std::cout << Colors::RED << "File does not exist: " << Colors::RESET << message << std::endl;
        }
    }

    return std::move(messages);
}
