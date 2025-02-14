#ifndef SERVER_FILE_HANDLER_H
#define SERVER_FILE_HANDLER_H

#include <fstream>
#include <string>

namespace Server
{
    class FileHandler
    {
    public:
        explicit FileHandler(const std::string& folder, const std::string& name);

        FileHandler(FileHandler&& other) noexcept;
        FileHandler& operator=(FileHandler&& other) noexcept;

        ~FileHandler();

        // Disallow copying and assignment.
        FileHandler(const FileHandler&) = delete;
        FileHandler& operator=(const FileHandler&) = delete;

        /**
         * @brief Writes data to the file.
         *
         * @param data Pointer to the data buffer containing the bytes to write.
         * @param size Number of bytes to write from the buffer.
         */

        void write(char* data, size_t size);

    private:
        std::string get_absolute_path(const std::string& folder, const std::string& name);

        std::ofstream _file;
    };
}

#endif  // SERVER_FILE_HANDLER_H