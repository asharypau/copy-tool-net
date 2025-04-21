#include "FileService.h"

#include <filesystem>

FileService::FileService()
    : _file()
{
}

void FileService::open_create(const std::string& path)
{
    const std::string version = get_version(path);
    _file.open(path + version, std::ios::out | std::ios::binary);

    if (!_file)
    {
        throw std::runtime_error("Failed to create file: " + path);
    }
}

void FileService::open_read(const std::string& path)
{
    _file.open(path, std::ios::in | std::ios::binary);

    if (!_file)
    {
        throw std::runtime_error("Failed to open file: " + path);
    }
}

void FileService::close()
{
    if (_file.is_open())
    {
        _file.close();
    }

    if (!_file)
    {
        throw std::runtime_error("Failed to close file");
    }
}

void FileService::write(const char* data, std::size_t size)
{
    _file.write(data, size);

    if (!_file)
    {
        throw std::runtime_error("Failed to write file");
    }
}

std::size_t FileService::read(char* data, std::size_t size)
{
    _file.read(data, size);

    if (!_file)
    {
        throw std::runtime_error("Failed to read file");
    }

    return _file.gcount();
}

std::string FileService::get_version(const std::string& path)
{
    std::string result;

    for (unsigned int i = 1; i < std::numeric_limits<unsigned int>::max(); ++i)
    {
        result = "_v" + std::to_string(i);

        if (!std::filesystem::exists(path + result))
        {
            break;
        }
    }

    return std::move(result);
}
