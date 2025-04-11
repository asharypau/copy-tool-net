#ifndef FILE_REQUEST_H
#define FILE_REQUEST_H

#include <vector>

#include "../common/network/tcp/Constants.h"
#include "../common/network/tcp/ISerializable.h"

class FileRequest : public Tcp::ISerializable<FileRequest>
{
public:
    Tcp::header_t batch_size = 0;
    std::vector<char> batch;

private:
    friend class Tcp::ISerializable<FileRequest>;

    std::vector<unsigned char> serialize_impl()
    {
        std::size_t offset = 0;
        std::vector<unsigned char> buffer(Tcp::HEADER_SIZE + batch_size);

        // batch_size
        std::memcpy(buffer.data() + offset, &batch_size, Tcp::HEADER_SIZE);
        offset += Tcp::HEADER_SIZE;

        // batch
        std::memcpy(buffer.data() + offset, batch.data(), batch_size);
        offset += batch_size;

        return std::move(buffer);
    }

    void deserialize_impl(const std::vector<unsigned char>& bytes)
    {
        Tcp::header_t offset = 0;
        const unsigned char* begin = bytes.data();

        // batch_size
        std::memcpy(&batch_size, begin + offset, Tcp::HEADER_SIZE);
        offset += Tcp::HEADER_SIZE;

        // batch
        batch.resize(batch_size);
        std::memcpy(batch.data(), begin + offset, batch_size);
        offset += batch_size;
    }
};

#endif  // FILE_REQUEST_H
