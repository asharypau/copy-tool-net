#include "FileService.h"

#include <filesystem>

#include "Startup.h"

using namespace Server;

FileService::FileService()
    : _file()
{
}

void FileService::create(const std::string& path, const std::string& name)
{
    _file.open(get_absolute_path(path, name), std::ios::binary);

    if (!_file)
    {
        throw std::runtime_error("Failed to create file: " + name);
    }
}

void FileService::open(const std::string& path)
{
    _file.open(path, std::ios::binary);

    if (!_file)
    {
        throw std::runtime_error("Failed to create file: " + path);
    }
}

void FileService::close()
{
    _file.close();
}

void FileService::write(char* data, size_t size)
{
    _file.write(data, size);
}

size_t FileService::read(char* data, size_t size)
{
    _file.read(data, size);

    return _file.gcount();
}

std::string FileService::get_absolute_path(const std::string& path, const std::string& name)
{
    std::string result;

    for (unsigned int i = 1; i < std::numeric_limits<int>::max(); ++i)
    {
        result = std::string(Server::CLIENT_STORAGE_PATH) + path + '/' + name + "_v" + std::to_string(i);

        if (!std::filesystem::exists(result))
        {
            break;
        }
    }

    return std::move(result);
}
