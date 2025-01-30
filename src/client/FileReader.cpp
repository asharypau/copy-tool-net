#include <filesystem>

#include "FileReader.h"


FileReader::FileReader(std::string file_name)
    : _file(file_name, std::ifstream::binary), _file_name(std::move(file_name))
{
    if (!_file)
    {
        throw std::runtime_error("Failed to open file: " + file_name);
    }
}

size_t FileReader::read(char* data, size_t size)
{
    _file.read(data, size);

    return _file.gcount();
}

size_t FileReader::get_size() const
{
    return std::filesystem::file_size(_file_name);
}
