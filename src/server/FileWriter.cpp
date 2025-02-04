#include "FileWriter.h"

#include <filesystem>
#include <string>
#include <utility>

void FileWriter::create(const std::string& file_name)
{
    std::string path = get_absolute_path(file_name);
    _file.open(path, std::ios::binary);
}

void FileWriter::close()
{
    _file.close();
}

void FileWriter::write(char* data, size_t size)
{
    _file.write(data, size);
}

std::string FileWriter::get_absolute_path(const std::string& file_name)
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
