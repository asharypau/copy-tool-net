#include "FileStorage.h"

#include <filesystem>
#include <format>
#include <utility>

#include "../utils/Logger.h"

using namespace Server;

FileStorage::FileStorage(const std::string& folder_name)
    : _absolute_path(create_directory(folder_name)),
      _file_service()
{
}

std::string FileStorage::create_directory(const std::string& folder_name) const
{
    std::string path = std::format("{}/{}/", std::string(FILE_STORAGE_PATH), folder_name);
    if (std::filesystem::exists(path))
    {
        Logger::info(std::format("File storage '{}' already exists", path));
    }
    else
    {
        std::filesystem::create_directory(path);
        Logger::info(std::format("File storage '{}' created", path));
    }

    return std::move(path);
}
