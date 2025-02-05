#include "FileHandler.h"

#include <filesystem>

FileHandler::FileHandler(size_t bytes_to_write)
    : _bytes_to_write(bytes_to_write)
{
}

FileHandler::~FileHandler()
{
    _file.close();
}

void FileHandler::create(const std::string& file_name)
{
    std::string path = get_absolute_path(file_name);
    _file.open(path, std::ios::binary);
}

void FileHandler::write(boost::asio::streambuf& buffer, size_t size)
{
    const char* raw_data = boost::asio::buffer_cast<const char*>(buffer.data());
    _file.write(raw_data, size);

    if (_file.good())
    {
        buffer.consume(size);
        _bytes_to_write -= size;
    }
}

std::string FileHandler::get_absolute_path(const std::string& file_name)
{
    std::string result;

    for (unsigned int i = 1; i < std::numeric_limits<int>::max(); ++i)
    {
        result = std::string(PATH) + file_name + std::to_string(i);

        if (!std::filesystem::exists(result))
        {
            break;
        }
    }

    return std::move(result);
}
