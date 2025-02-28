#include "FileController.h"

#include "../common/models/FileHeaders.h"
#include "../common/models/FileRequest.h"

using namespace Server;

FileController::FileController(Tcp::Reader& tcp_reader, Tcp::Writer& tcp_writer, const StorageProvider& storage_provider)
    : _tcp_reader(tcp_reader),
      _tcp_writer(tcp_writer),
      _storage_provider(storage_provider),
      _file_service(),
      _request_size(0)
{
}

boost::asio::awaitable<void> FileController::handle(Tcp::header_t request_size)
{
    if (request_size != 0)
    {
        _request_size = request_size;
        co_await read_headers();
    }
}

boost::asio::awaitable<void> FileController::read_headers()
{
    FileHeaders file_headers = co_await _tcp_reader.read_async<FileHeaders>();
    _request_size -= file_headers.get_content_length();

    std::string path = std::format("{}{}", _storage_provider.get_path(), file_headers.name);
    _file_service.open_create(path);

    co_await read_file();
    co_await write_confirmation(file_headers.confirmation_id);
}

boost::asio::awaitable<void> FileController::read_file()
{
    FileRequest file_request = co_await _tcp_reader.read_async<FileRequest>();
    _request_size -= file_request.get_content_length();

    _file_service.write(file_request.batch.data(), file_request.batch_size);

    if (_request_size > 0)
    {
        co_await read_file();
    }
    else
    {
        _file_service.close();
    }
}

boost::asio::awaitable<void> FileController::write_confirmation(Tcp::header_t confirmation_id)
{
    co_await _tcp_writer.write_async(confirmation_id);
}
