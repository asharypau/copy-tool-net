#ifndef FILE_SERVICE_H
#define FILE_SERVICE_H

#include <fstream>
#include <string>

namespace Server
{
    class FileService
    {
    public:
        FileService();

        void create(const std::string& path, const std::string& name);
        void open(const std::string& path);
        void close();
        void write(char* data, size_t size);
        size_t read(char* date, size_t size);

    private:
        std::string get_absolute_path(const std::string& path, const std::string& name);

        std::fstream _file;
    };
}

#endif  // FILE_SERVICE_H