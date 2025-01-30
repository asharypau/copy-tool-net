#include "FileWriter.h"

size_t FileWriter::_file_id = 1;

FileWriter::FileWriter(std::string path)
    : _file(get_absolute_path(path), std::ios::binary)
{
    if (_file)
    {
        ++_file_id;
    }
}

void FileWriter::write(char* data, size_t size)
{
    _file.write(data, size);
}

std::string FileWriter::get_absolute_path(std::string path)
{
    if (path.back() != '/')
    {
        path += '/';
    }

    return path + std::to_string(_file_id) + ".txt";
}
