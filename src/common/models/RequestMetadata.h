#ifndef REQUEST_METADATA_H
#define REQUEST_METADATA_H

#include <cstddef>
#include <string>
#include <vector>

#include "../network/tcp/ISerializable.h"

class RequestMetadata : public Tcp::ISerializable
{
public:
    std::vector<std::byte> serialize() override
    {
        size_t offset = 0;
        std::vector<std::byte> buffer(Tcp::HEADER_SIZE * 2 + endpoint.size());

        // size
        std::memcpy(buffer.data() + offset, &size, Tcp::HEADER_SIZE);
        offset += Tcp::HEADER_SIZE;

        // endpoint size
        Tcp::header_t endpoint_size = endpoint.size();
        std::memcpy(buffer.data() + offset, &endpoint_size, Tcp::HEADER_SIZE);
        offset += Tcp::HEADER_SIZE;

        // endpoint
        std::memcpy(buffer.data() + offset, endpoint.data(), endpoint.size());
        offset += endpoint.size();

        return std::move(buffer);
    }

    Tcp::header_t deserialize(boost::beast::flat_buffer& buffer) override
    {
        Tcp::header_t offset = 0;
        char* begin = static_cast<char*>(buffer.data().data());

        // size
        std::memcpy(&size, begin + offset, Tcp::HEADER_SIZE);
        offset += Tcp::HEADER_SIZE;

        // endpoint size
        Tcp::header_t endpoint_size = 0;
        std::memcpy(&endpoint_size, begin + offset, Tcp::HEADER_SIZE);
        offset += Tcp::HEADER_SIZE;

        // endpoint
        endpoint.assign(begin + offset, endpoint_size);
        offset += endpoint_size;

        return offset;
    }

    Tcp::header_t size = 0;
    std::string endpoint;
};

#endif  // REQUEST_METADATA_H
