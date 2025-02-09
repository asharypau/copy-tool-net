#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <fstream>
#include <string>
#include <string_view>

class FileHandler
{
public:
    explicit FileHandler(size_t bytes_to_write, const std::string& name);

    FileHandler(FileHandler&& other) noexcept;
    FileHandler& operator=(FileHandler&& other) noexcept;

    ~FileHandler();

    // Disallow copying and assignment.
    FileHandler(const FileHandler&) = delete;
    FileHandler& operator=(const FileHandler&) = delete;

    void write(char* data, size_t size);

    size_t get_bytes_to_write() const noexcept
    {
        return _bytes_to_write;
    }

private:
    std::string get_absolute_path(const std::string& file_name);

    static constexpr std::string_view PATH = "D:/Education/CppMentoring/files/output/";

    std::ofstream _file;
    size_t _bytes_to_write;
};

#endif  // FILE_HANDLER_H