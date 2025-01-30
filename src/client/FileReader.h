#ifndef FILE_READER_H
#define FILE_READER_H

#include <fstream>
#include <string>

class FileReader
{
public:
    explicit FileReader(std::string file_name);

    size_t read(char* date, size_t size);
    size_t get_size() const;

private:
    std::ifstream _file;
    std::string _file_name;
};

#endif  // FILE_READER_H
