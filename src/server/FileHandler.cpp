#include "FileHandler.h"

size_t FileHandler::_file_id = 0;

FileHandler::FileHandler(std::string path)
    : _file(get_absolute_path(path), std::ios::binary)
{
    if (_file)
    {
        ++_file_id;
    }
}

void FileHandler::write(char* data, size_t size)
{
    _file.write(data, size);
}

std::string FileHandler::get_absolute_path(std::string path)
{
    if (path.back() != '/')
    {
        path += '/';
    }

    return path + std::to_string(_file_id) + ".txt";
}
