#ifndef FILE_HEADER_H
#define FILE_HEADER_H

#include <cstddef>
#include <string>

#include "../network/tcp/ISerializable.h"

class FileHeaders : public Tcp::ISerializable
{
public:
    std::vector<std::byte> serialize() override
    {
        size_t offset = 0;
        std::vector<std::byte> buffer(Tcp::HEADER_SIZE * 2 + name.size());

        // confirmation_id
        std::memcpy(buffer.data() + offset, &confirmation_id, Tcp::HEADER_SIZE);
        offset += Tcp::HEADER_SIZE;

        // name size
        Tcp::header_t name_size = name.size();
        std::memcpy(buffer.data() + offset, &name_size, Tcp::HEADER_SIZE);
        offset += Tcp::HEADER_SIZE;

        // name
        std::memcpy(buffer.data() + offset, name.data(), name_size);
        offset += name_size;

        return std::move(buffer);
    }

    Tcp::header_t deserialize(boost::beast::flat_buffer& buffer) override
    {
        Tcp::header_t offset = 0;
        char* begin = static_cast<char*>(buffer.data().data());

        // confirmation_id
        std::memcpy(&confirmation_id, begin + offset, Tcp::HEADER_SIZE);
        offset += Tcp::HEADER_SIZE;

        // name size
        Tcp::header_t name_size = 0;
        std::memcpy(&name_size, begin + offset, Tcp::HEADER_SIZE);
        offset += Tcp::HEADER_SIZE;

        // name
        name.assign(begin + offset, name_size);
        offset += name_size;

        return offset;
    }

    Tcp::header_t confirmation_id = 0;
    std::string name;
};

#endif  // FILE_HEADER_H
