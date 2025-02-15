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

        /**
         * @brief Reads data from the file into the provided buffer.
         *
         * @param data Pointer to the buffer where the read data will be stored.
         * @param size Maximum number of bytes to read from the file.
         * @return size_t The actual number of bytes read.
         */
        size_t read(char* date, size_t size);

    private:
        std::fstream _file;
    };
}

#endif  // CLIENT_FILE_HANDLER_H
