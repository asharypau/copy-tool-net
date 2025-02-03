#include "FileWriter.h"

size_t FileWriter::_file_id = 1;

void FileWriter::open(const std::string& file_name)
{
    _file.open(get_absolute_path("D:/Education/CppMentoring/files/output"), std::ios::binary);
    ++_file_id;
}

void FileWriter::close()
{
    _file.close();
}

void FileWriter::write(char* data, size_t size)
{
    _file.write(data, size);
}

bool FileWriter::is_open() const
{
    return _file.is_open();
}

std::string FileWriter::get_absolute_path(std::string path)
{
    if (path.back() != '/')
    {
        path += '/';
    }

    return path + std::to_string(_file_id) + ".txt";
}
