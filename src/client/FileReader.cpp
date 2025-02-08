#include "FileReader.h"

FileReader::FileReader(const std::string& path)
    : _file(path, std::ifstream::binary)
{
    if (!_file)
    {
        throw std::runtime_error("Failed to open file: " + path);
    }
}

FileReader::FileReader(FileReader&& other) noexcept
    : _file(std::move(other._file))
{
}

FileReader& FileReader::operator=(FileReader&& other) noexcept
{
    if (this != &other)
    {
        _file = std::move(other._file);
    }

    return *this;
}

FileReader::~FileReader()
{
    _file.close();
}

size_t FileReader::read(char* data, size_t size)
{
    _file.read(data, size);

    return _file.gcount();
}
