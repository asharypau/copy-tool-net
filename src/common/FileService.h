#ifndef FILE_SERVICE_H
#define FILE_SERVICE_H

#include <fstream>
#include <string>

class FileService
{
public:
    FileService();

    void open_create(const std::string& path, const std::string& name);
    void open_read(const std::string& path);
    void close();
    void write(char* data, size_t size);
    size_t read(char* date, size_t size);

private:
    std::string get_absolute_path(const std::string& path, const std::string& name);

    std::fstream _file;
};

#endif  // FILE_SERVICE_H