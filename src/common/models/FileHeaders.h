#ifndef FILE_HEADER_H
#define FILE_HEADER_H

#include <cstddef>
#include <string>

#include "../network/tcp/ISerializable.h"

class FileHeaders : public Tcp::ISerializable
{
public:
    FileHeaders() = default;

    FileHeaders(Tcp::header_t content_length)
        : ISerializable(content_length)
    {
    }

    std::vector<std::byte> serialize() override
    {
        size_t offset = 0;
        std::vector<std::byte> buffer(Tcp::HEADER_SIZE * 2 + name.size());

        std::memcpy(buffer.data() + offset, &confirmation_id, Tcp::HEADER_SIZE);
        offset += Tcp::HEADER_SIZE;

        Tcp::header_t name_size = name.size();
        std::memcpy(buffer.data() + offset, &name_size, Tcp::HEADER_SIZE);
        offset += Tcp::HEADER_SIZE;

        std::memcpy(buffer.data() + offset, name.data(), name_size);
        offset += name_size;

        return std::move(buffer);
    }

    Tcp::header_t deserialize(const boost::asio::streambuf& buffer) override
    {
        Tcp::header_t offset = 0;

        const Tcp::header_t* raw_confirmation_id = boost::asio::buffer_cast<const Tcp::header_t*>(buffer.data() + offset);
        std::memcpy(&confirmation_id, raw_confirmation_id, Tcp::HEADER_SIZE);
        offset += Tcp::HEADER_SIZE;

        const Tcp::header_t* raw_name_size = boost::asio::buffer_cast<const Tcp::header_t*>(buffer.data() + offset);
        offset += Tcp::HEADER_SIZE;

        const char* raw_name = boost::asio::buffer_cast<const char*>(buffer.data() + offset);
        name.assign(raw_name, *raw_name_size);
        offset += *raw_name_size;

        return offset;
    }

    Tcp::header_t confirmation_id = 0;
    std::string name;
};

#endif  // FILE_HEADER_H
