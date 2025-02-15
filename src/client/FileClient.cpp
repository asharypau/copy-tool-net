#include "FileClient.h"

#include "../models/Endpoints.h"
#include "../models/FileHeaders.h"
#include "../models/FileRequest.h"
#include "../models/RequestMetadata.h"

using namespace Client;

FileClient::FileClient(boost::asio::ip::tcp::socket& socket)
    : _tcp_writer(socket),
      _tcp_reader(socket),
      _file_service(),
      _write_handle(),
      _read_handle()
{
}

void FileClient::write(Message message)
{
    size_t batches_count = (message.size + BATCH_SIZE - 1) / BATCH_SIZE;

    RequestMetadata request_metadata;
    request_metadata.endpoint = Endpoints::FILE;
    request_metadata.size = (Tcp::HEADER_SIZE * 2 + message.name.size()) + (batches_count * Tcp::HEADER_SIZE + message.size);

    _tcp_writer.write_async(
        request_metadata,
        [this, message]
        {
            write_headers(message);
        });
}

void FileClient::read()
{
    _tcp_reader.read_async<Tcp::header_t>(
        [this](Tcp::header_t confirmation_id)
        {
            if (_read_handle.has_value())
            {
                _read_handle.value()(confirmation_id);
            }
        });
}

void FileClient::write_headers(Message message)
{
    FileHeaders headers;
    headers.confirmation_id = message.id;
    headers.name = message.name;

    _tcp_writer.write_async(
        headers,
        [this, message]
        {
            _file_service.open_read(message.path);
            write_file();
        });
}

void FileClient::write_file()
{
    FileRequest request;
    request.batch.resize(BATCH_SIZE);
    request.batch_size = _file_service.read(request.batch.data(), BATCH_SIZE);

    if (request.batch_size > 0)
    {
        _tcp_writer.write_async(
            request,
            [this]()
            {
                write_file();
            });
    }
    else
    {
        _file_service.close();

        if (_write_handle.has_value())
        {
            _write_handle.value()();
        }
    }
}