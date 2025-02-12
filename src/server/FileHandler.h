#ifndef SERVER_FILE_HANDLER_H
#define SERVER_FILE_HANDLER_H

#include <fstream>
#include <string>

namespace Server
{
    class FileHandler
    {
    public:
        explicit FileHandler(size_t id, const std::string& folder, const std::string& name, size_t bytes_to_write);

        FileHandler(FileHandler&& other) noexcept;
        FileHandler& operator=(FileHandler&& other) noexcept;

        ~FileHandler();

        // Disallow copying and assignment.
        FileHandler(const FileHandler&) = delete;
        FileHandler& operator=(const FileHandler&) = delete;

        void write(char* data, size_t size);

        size_t* get_id() noexcept
        {
            return &_id;
        }

        size_t get_bytes_to_write() const noexcept
        {
            return _bytes_to_write;
        }

    private:
        std::string get_absolute_path(const std::string& folder, const std::string& name);

        std::ofstream _file;
        size_t _id;
        size_t _bytes_to_write;
    };
}

#endif  // SERVER_FILE_HANDLER_H