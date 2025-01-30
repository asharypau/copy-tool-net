#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <fstream>
#include <string>

class FileWriter
{
public:
    explicit FileWriter(std::string path);

    void write(char* date, size_t size);

private:
    std::string get_absolute_path(std::string path);

    static size_t _file_id;
    std::ofstream _file;
};

#endif  // FILE_HANDLER_H