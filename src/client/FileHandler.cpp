#include "FileHandler.h"

using namespace Client;

FileHandler::FileHandler(const std::string& path)
    : _file(path, std::ifstream::binary)
{
    if (!_file)
    {
        throw std::runtime_error("Failed to open file: " + path);
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

size_t FileHandler::read(char* data, size_t size)
{
    _file.read(data, size);

    return _file.gcount();
}
