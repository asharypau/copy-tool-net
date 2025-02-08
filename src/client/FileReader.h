#ifndef FILE_READER_H
#define FILE_READER_H

#include <fstream>
#include <string>

class FileReader
{
public:
    explicit FileReader(const std::string& path);

    FileReader(FileReader&& other) noexcept;
    FileReader& operator=(FileReader&& other) noexcept;

    ~FileReader();

    // Disallow copying and assignment.
    FileReader(const FileReader&) = delete;
    FileReader& operator=(const FileReader&) = delete;

    size_t read(char* date, size_t size);

private:
    std::ifstream _file;
};

#endif  // FILE_READER_H
