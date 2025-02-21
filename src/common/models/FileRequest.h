#ifndef FILE_REQUEST_H
#define FILE_REQUEST_H

#include <vector>

#include "../network/tcp/ISerializable.h"

class FileRequest : public Tcp::ISerializable
{
public:
    FileRequest() = default;

    FileRequest(Tcp::header_t content_length)
        : ISerializable(content_length)
    {
    }

    std::vector<std::byte> serialize() override
    {
        size_t offset = 0;
        std::vector<std::byte> buffer(Tcp::HEADER_SIZE + batch_size);

        // batch_size
        std::memcpy(buffer.data() + offset, &batch_size, Tcp::HEADER_SIZE);
        offset += Tcp::HEADER_SIZE;

        // batch
        std::memcpy(buffer.data() + offset, batch.data(), batch_size);
        offset += batch_size;

        return std::move(buffer);
    }

    Tcp::header_t deserialize(boost::beast::flat_buffer& buffer) override
    {
        Tcp::header_t offset = 0;
        char* begin = static_cast<char*>(buffer.data().data());

        // batch_size
        std::memcpy(&batch_size, begin + offset, Tcp::HEADER_SIZE);
        offset += Tcp::HEADER_SIZE;

        // batch
        batch.resize(batch_size);
        std::memcpy(batch.data(), begin + offset, batch_size);
        offset += batch_size;

        return offset;
    }

    Tcp::header_t batch_size = 0;
    std::vector<char> batch;
};

#endif  // FILE_REQUEST_H
