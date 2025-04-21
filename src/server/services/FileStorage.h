#ifndef FILE_STORAGE_H
#define FILE_STORAGE_H

#include <cstddef>
#include <format>
#include <string>

#include "../common/services/FileService.h"

namespace Server
{
    class FileStorage
    {
    public:
        explicit FileStorage(const std::string& folder_name);

        /**
         * @brief Opens or creates a file with the specified name.
         */
        void open_create(const std::string& name)
        {
            _file_service.open_create(std::format("{}{}", _absolute_path, name));
        }

        /**
         * @brief Writes data to a file.
         *
         * @param data Pointer to the data to be written.
         * @param size The size of the data to be written, in bytes.
         */
        void write(const char* data, std::size_t size)
        {
            _file_service.write(data, size);
        }

        /**
         * @brief Closes the file.
         */
        void close()
        {
            _file_service.close();
        }

    private:
        /**
         * @brief Creates a directory with the specified name.
         *
         * @param folder_name The name of the directory to be created.
         *
         * @return std::string A string containing the path of the created directory.
         */
        std::string create_directory(const std::string& folder_name) const;

        static constexpr std::string_view FILE_STORAGE_PATH = "D:/Education/CppMentoring/files/output";

        std::string _absolute_path;
        FileService _file_service;
    };
}

#endif  // FILE_STORAGE_H
