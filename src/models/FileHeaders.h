#ifndef FILE_HEADER_H
#define FILE_HEADER_H

#include <cstddef>
#include <string>

#include "../common/network/tcp/Constants.h"
#include "../common/network/tcp/ISerializable.h"

class FileHeaders : public Tcp::ISerializable<FileHeaders>
{
public:
    Tcp::header_t confirmation_id = 0;
    std::string name;

private:
    friend class Tcp::ISerializable<FileHeaders>;

    std::vector<unsigned char> serialize_impl()
    {
        std::size_t offset = 0;
        std::vector<unsigned char> buffer(Tcp::HEADER_SIZE * 2 + name.size());

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

    void deserialize_impl(const std::vector<unsigned char>& bytes)
    {
        Tcp::header_t offset = 0;
        const unsigned char* begin = bytes.data();

        // confirmation_id
        std::memcpy(&confirmation_id, begin + offset, Tcp::HEADER_SIZE);
        offset += Tcp::HEADER_SIZE;

        // name size
        Tcp::header_t name_size = 0;
        std::memcpy(&name_size, begin + offset, Tcp::HEADER_SIZE);
        offset += Tcp::HEADER_SIZE;

        // name
        name = std::string(begin + offset, begin + offset + name_size);
        offset += name_size;
    }
};

#endif  // FILE_HEADER_H
