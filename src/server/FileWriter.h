#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <fstream>
#include <string>

class FileWriter
{
public:
    void open(const std::string& file_name);
    void close();
    void write(char* date, size_t size);
    bool is_open() const;

private:
    std::string get_absolute_path(std::string path);

    static size_t _file_id;
    std::ofstream _file;
};

#endif  // FILE_HANDLER_H