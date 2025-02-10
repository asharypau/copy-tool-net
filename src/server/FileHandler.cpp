#include "FileHandler.h"

#include <filesystem>

using namespace Server;

FileHandler::FileHandler(size_t bytes_to_write, const std::string& name)
    : _file(get_absolute_path(name), std::ios::binary),
      _bytes_to_write(bytes_to_write)
{
    if (!_file)
    {
        throw std::runtime_error("Failed to create file: " + name);
    }
}

FileHandler::FileHandler(FileHandler&& other) noexcept
    : _file(std::move(other._file)),
      _bytes_to_write(other._bytes_to_write)
{
}

FileHandler& FileHandler::operator=(FileHandler&& other) noexcept
{
    if (this != &other)
    {
        _file = std::move(other._file);
        _bytes_to_write = other._bytes_to_write;
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

    if (_file.good())
    {
        _bytes_to_write -= size;
    }
}

std::string FileHandler::get_absolute_path(const std::string& file_name)
{
    std::string result;

    for (unsigned int i = 1; i < std::numeric_limits<int>::max(); ++i)
    {
        result = std::string(PATH) + file_name + std::to_string(i);

        if (!std::filesystem::exists(result))
        {
            break;
        }
    }

    return std::move(result);
}
