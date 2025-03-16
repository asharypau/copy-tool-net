#ifndef FILE_SERVICE_H
#define FILE_SERVICE_H

#include <fstream>
#include <string>

class FileService
{
public:
    FileService();

    void open_create(const std::string& path);
    void open_read(const std::string& path);
    void close();
    void write(const char* data, size_t size);
    size_t read(char* date, size_t size);

private:
    std::string get_version(const std::string& path);

    std::fstream _file;
};

#endif  // FILE_SERVICE_H