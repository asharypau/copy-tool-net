#include "FileClient.h"

#include "../common/models/FileHeaders.h"
#include "../common/models/FileRequest.h"
#include "../common/models/RequestMetadata.h"
#include "../common/network/Endpoints.h"

using namespace Client;

FileClient::FileClient(boost::asio::ip::tcp::socket& socket)
    : _tcp_writer(socket),
      _tcp_reader(socket),
      _file_service()
{
}

boost::asio::awaitable<void> FileClient::write(const Message& message)
{
    size_t batches_count = (message.size + BATCH_SIZE - 1) / BATCH_SIZE;

    RequestMetadata request_metadata;
    request_metadata.endpoint = Endpoints::FILE;
    request_metadata.size = (Tcp::HEADER_SIZE * 2 + message.name.size()) + (batches_count * Tcp::HEADER_SIZE + message.size);

    co_await _tcp_writer.write_async(request_metadata);
    co_await write_headers(message);
    co_await write_file(message);
}

boost::asio::awaitable<Tcp::header_t> FileClient::read_confirmation()
{
    auto [confirmation_id, _] = co_await _tcp_reader.read_async<Tcp::header_t>();

    co_return confirmation_id;
}

boost::asio::awaitable<void> FileClient::write_headers(const Message& message)
{
    FileHeaders headers;
    headers.confirmation_id = message.id;
    headers.name = message.name;

    co_await _tcp_writer.write_async(headers);
}

boost::asio::awaitable<void> FileClient::write_file(const Message& message)
{
    size_t remaining_bytes_to_read = message.size;
    _file_service.open_read(message.path);

    while (true)
    {
        FileRequest request;
        request.batch_size = remaining_bytes_to_read > BATCH_SIZE ? BATCH_SIZE : remaining_bytes_to_read;
        request.batch.resize(request.batch_size);

        _file_service.read(request.batch.data(), request.batch_size);
        co_await _tcp_writer.write_async(request);

        remaining_bytes_to_read -= request.batch_size;
        if (remaining_bytes_to_read == 0)
        {
            break;
        }
    }

    _file_service.close();
}