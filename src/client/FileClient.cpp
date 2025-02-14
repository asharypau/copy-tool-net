#include "FileClient.h"

#include "../models/Endpoints.h"

using namespace Client;

FileClient::FileClient(boost::asio::ip::tcp::socket& socket)
    : _headers(),
      _batch(Tcp::HEADER_SIZE + BATCH_SIZE),
      _tcp_writer(socket),
      _tcp_reader(socket),
      _write_handle(),
      _read_handle()
{
}

void FileClient::write(Message message)
{
    size_t batches_count = (message.size + BATCH_SIZE - 1) / BATCH_SIZE;
    size_t headers_count = 2 + batches_count;
    size_t request_size = Tcp::HEADER_SIZE * headers_count + message.name.size() + message.size;

    _headers.resize(Endpoints::LENGTH + Tcp::HEADER_SIZE);
    std::memset(_headers.data(), 0, _headers.size());                                   // reset headers
    std::memcpy(_headers.data(), Endpoints::FILE.data(), Endpoints::FILE.size());       // write an endpoint name into the buffer at index 0
    std::memcpy(_headers.data() + Endpoints::LENGTH, &request_size, Tcp::HEADER_SIZE);  // write a request size into the buffer at index Endpoints::LENGTH

    _tcp_writer.write(
        _headers.data(),
        _headers.size(),
        [this, message]
        {
            write_headers(message);
        });
}

void FileClient::read()
{
    _tcp_reader.read_async(
        Tcp::HEADER_SIZE,
        [this]
        {
            size_t id;
            _tcp_reader.extract(&id, Tcp::HEADER_SIZE);

            if (_read_handle.has_value())
            {
                _read_handle.value()(id);
            }
        });
}

void FileClient::write_headers(Message message)
{
    size_t file_name_size = message.name.size();

    _headers.resize(Tcp::HEADER_SIZE * 2 + file_name_size);
    std::memset(_headers.data(), 0, _headers.size());                                          // reset headers
    std::memcpy(_headers.data(), &message.id, Tcp::HEADER_SIZE);                               // write a file id into the buffer at index 0
    std::memcpy(_headers.data() + Tcp::HEADER_SIZE, &file_name_size, Tcp::HEADER_SIZE);        // write a file name size into the buffer at index 0 + Tcp::HEADER_SIZE
    std::memcpy(_headers.data() + Tcp::HEADER_SIZE * 2, message.name.data(), file_name_size);  // write a name into the buffer at index 0 + Tcp::HEADER_SIZE * 2

    _tcp_writer.write(
        _headers.data(),
        _headers.size(),
        [this, message]
        {
            write_file(std::make_unique<FileHandler>(message.path));
        });
}

void FileClient::write_file(std::unique_ptr<FileHandler>&& file)
{
    size_t batch_size = file->read(_batch.data() + Tcp::HEADER_SIZE, BATCH_SIZE);  // write data into the buffer at index 0 + Tcp::HEADER_SIZE
    if (batch_size > 0)
    {
        std::memcpy(_batch.data(), &batch_size, Tcp::HEADER_SIZE);  // write batch size into the buffer at index 0

        _tcp_writer.write(
            _batch.data(),
            Tcp::HEADER_SIZE + batch_size,
            [this, file = std::move(file)]() mutable
            {
                write_file(std::move(file));
            });
    }
    else
    {
        if (_write_handle.has_value())
        {
            _write_handle.value()();
        }
    }
}