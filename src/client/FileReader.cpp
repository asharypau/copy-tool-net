#include "FileReader.h"

FileReader::FileReader(std::string name, std::string path)
    : _file(path, std::ifstream::binary),
      _name(std::move(name)),
      _path(std::move(path))
{
    if (!_file)
    {
        throw std::runtime_error("Failed to open file: " + path);
    }
}

size_t FileReader::read(char* data, size_t size)
{
    _file.read(data, size);

    return _file.gcount();
}
