#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <fstream>
#include <string>
#include <string_view>

class FileWriter
{
public:
    void create(const std::string& file_name);
    void close();
    void write(char* date, size_t size);

private:
    std::string get_absolute_path(const std::string& file_name);

    static constexpr std::string_view PATH = "D:/Education/CppMentoring/files/output/";

    std::ofstream _file;
};

#endif  // FILE_HANDLER_H