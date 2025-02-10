#ifndef CLIENT_FILE_HANDLER_H
#define CLIENT_FILE_HANDLER_H

#include <fstream>
#include <string>

namespace Client
{
    class FileHandler
    {
    public:
        explicit FileHandler(const std::string& path);

        FileHandler(FileHandler&& other) noexcept;
        FileHandler& operator=(FileHandler&& other) noexcept;

        ~FileHandler();

        // Disallow copying and assignment.
        FileHandler(const FileHandler&) = delete;
        FileHandler& operator=(const FileHandler&) = delete;

        size_t read(char* date, size_t size);

    private:
        std::ifstream _file;
    };
}

#endif  // CLIENT_FILE_HANDLER_H
