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

        void open_create(const std::string& name)
        {
            _file_service.open_create(std::format("{}{}", _absolute_path, name));
        }

        void write(const char* data, size_t size)
        {
            _file_service.write(data, size);
        }

        void close()
        {
            _file_service.close();
        }

    private:
        std::string create_directory(const std::string& folder_name) const;

        static constexpr std::string_view FILE_STORAGE_PATH = "D:/Education/CppMentoring/files/output";

        std::string _absolute_path;
        FileService _file_service;
    };
}

#endif  // FILE_STORAGE_H
