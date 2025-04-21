#ifndef REQUEST_METADATA_H
#define REQUEST_METADATA_H

#include <cstddef>
#include <string>
#include <vector>

#include "../common/network/tcp/Constants.h"
#include "../common/network/tcp/ISerializable.h"

class RequestMetadata : public Tcp::ISerializable<RequestMetadata>
{
public:
    Tcp::header_t size = 0;
    std::string endpoint;

private:
    friend class Tcp::ISerializable<RequestMetadata>;

    std::vector<unsigned char> serialize_impl()
    {
        std::size_t offset = 0;
        std::vector<unsigned char> buffer(Tcp::HEADER_SIZE * 2 + endpoint.size());

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

    void deserialize_impl(const std::vector<unsigned char>& bytes)
    {
        Tcp::header_t offset = 0;
        const unsigned char* begin = bytes.data();

        // size
        std::memcpy(&size, begin + offset, Tcp::HEADER_SIZE);
        offset += Tcp::HEADER_SIZE;

        // endpoint size
        Tcp::header_t endpoint_size = 0;
        std::memcpy(&endpoint_size, begin + offset, Tcp::HEADER_SIZE);
        offset += Tcp::HEADER_SIZE;

        // endpoint
        endpoint = std::string(begin + offset, begin + offset + endpoint_size);
        offset += endpoint_size;
    }
};

#endif  // REQUEST_METADATA_H
