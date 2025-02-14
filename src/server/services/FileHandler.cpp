#include "FileHandler.h"

#include <filesystem>
#include <string>

#include "Startup.h"

using namespace Server;

FileHandler::FileHandler(const std::string& folder, const std::string& name)
    : _file(get_absolute_path(folder, name), std::ios::binary)
{
    if (!_file)
    {
        throw std::runtime_error("Failed to create file: " + name);
    }
}

FileHandler::FileHandler(FileHandler&& other) noexcept
    : _file(std::move(other._file))
{
}

FileHandler& FileHandler::operator=(FileHandler&& other) noexcept
{
    if (this != &other)
    {
        _file = std::move(other._file);
    }

    return *this;
}

FileHandler::~FileHandler()
{
    _file.close();
}

void FileHandler::write(char* data, size_t size)
{
    _file.write(data, size);
}

std::string FileHandler::get_absolute_path(const std::string& folder, const std::string& name)
{
    std::string result;

    for (unsigned int i = 1; i < std::numeric_limits<int>::max(); ++i)
    {
        result = std::string(Server::CLIENT_STORAGE_PATH) + folder + '/' + name + "_v" + std::to_string(i);

        if (!std::filesystem::exists(result))
        {
            break;
        }
    }

    return std::move(result);
}
