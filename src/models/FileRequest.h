#ifndef FILE_REQUEST_H
#define FILE_REQUEST_H

#include <vector>

#include "../common/Tcp.h"

class FileRequest : public Tcp::ISerializable
{
public:
    FileRequest() = default;

    FileRequest(Tcp::header_type content_length)
        : ISerializable(content_length)
    {
    }

    std::vector<char> serialize() override
    {
        size_t offset = 0;
        std::vector<char> buffer(Tcp::HEADER_SIZE + batch_size);

        std::memcpy(buffer.data() + offset, reinterpret_cast<const char*>(&batch_size), Tcp::HEADER_SIZE);
        offset += Tcp::HEADER_SIZE;

        std::memcpy(buffer.data() + offset, batch.data(), batch_size);
        offset += batch_size;

        return std::move(buffer);
    }

    Tcp::header_type deserialize(const boost::asio::streambuf& buffer) override
    {
        Tcp::header_type offset = 0;

        const Tcp::header_type* raw_batch_size = boost::asio::buffer_cast<const Tcp::header_type*>(buffer.data() + offset);
        std::memcpy(&batch_size, raw_batch_size, Tcp::HEADER_SIZE);
        offset += Tcp::HEADER_SIZE;

        const char* raw_batch = boost::asio::buffer_cast<const char*>(buffer.data() + offset);
        batch.resize(batch_size);
        std::memcpy(batch.data(), raw_batch, batch_size);
        offset += batch_size;

        return offset;
    }

    Tcp::header_type batch_size = 0;
    std::vector<char> batch;
};

#endif  // FILE_REQUEST_H
