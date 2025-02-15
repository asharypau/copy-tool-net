#ifndef REQUEST_METADATA_H
#define REQUEST_METADATA_H

#include <string>
#include <vector>

#include "../common/Tcp.h"

class RequestMetadata : public Tcp::ISerializable
{
public:
    RequestMetadata() = default;

    RequestMetadata(Tcp::header_t content_length)
        : ISerializable(content_length)
    {
    }

    std::vector<char> serialize() override
    {
        size_t offset = 0;
        std::vector<char> buffer(Tcp::HEADER_SIZE * 2 + endpoint.size());

        std::memcpy(buffer.data() + offset, reinterpret_cast<const char*>(&size), Tcp::HEADER_SIZE);
        offset += Tcp::HEADER_SIZE;

        Tcp::header_t endpoint_size = endpoint.size();
        std::memcpy(buffer.data() + offset, reinterpret_cast<const char*>(&endpoint_size), Tcp::HEADER_SIZE);
        offset += Tcp::HEADER_SIZE;

        std::memcpy(buffer.data() + offset, endpoint.data(), endpoint.size());
        offset += endpoint.size();

        return std::move(buffer);
    }

    Tcp::header_t deserialize(const boost::asio::streambuf& buffer) override
    {
        Tcp::header_t offset = 0;

        const Tcp::header_t* raw_size = boost::asio::buffer_cast<const Tcp::header_t*>(buffer.data() + offset);
        std::memcpy(&size, raw_size, Tcp::HEADER_SIZE);
        offset += Tcp::HEADER_SIZE;

        const Tcp::header_t* raw_endpoint_size = boost::asio::buffer_cast<const Tcp::header_t*>(buffer.data() + offset);
        const Tcp::header_t endpoint_size = *raw_endpoint_size;
        offset += Tcp::HEADER_SIZE;

        const char* raw_endpoint = boost::asio::buffer_cast<const char*>(buffer.data() + offset);
        endpoint.assign(raw_endpoint, endpoint_size);
        offset += endpoint_size;

        return offset;
    }

    Tcp::header_t size = 0;
    std::string endpoint;
};

#endif  // REQUEST_METADATA_H
