#ifndef FILE_READER_H
#define FILE_READER_H

#include <filesystem>
#include <fstream>
#include <string>

class FileReader
{
public:
    FileReader(std::string name, std::string path);

    size_t read(char* date, size_t size);

    size_t get_size() const
    {
        return std::filesystem::file_size(_path);
    }

    const std::string& get_name() const noexcept
    {
        return _name;
    }

private:
    std::ifstream _file;
    std::string _name;
    std::string _path;
};

#endif  // FILE_READER_H
