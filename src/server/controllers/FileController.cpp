#include "FileController.h"

#include "../common/models/FileRequest.h"

using namespace Server;

FileController::FileController(Tcp::Reader& tcp_reader, Tcp::Writer& tcp_writer, FileStorage& file_storage)
    : _request_size(0),
      _tcp_reader(tcp_reader),
      _tcp_writer(tcp_writer),
      _file_storage(file_storage)
{
}

boost::asio::awaitable<void> FileController::handle(Tcp::header_t request_size)
{
    _request_size = request_size;

    FileHeaders headers = co_await read_headers();
    co_await read_file(headers);
    co_await write_confirmation(headers.confirmation_id);
}

boost::asio::awaitable<FileHeaders> FileController::read_headers()
{
    FileHeaders file_headers = co_await _tcp_reader.read_async<FileHeaders>();
    _request_size -= file_headers.get_content_length();

    co_return file_headers;
}

boost::asio::awaitable<void> FileController::read_file(FileHeaders& headers)
{
    _file_storage.open_create(headers.name);

    while (true)
    {
        FileRequest file_request = co_await _tcp_reader.read_async<FileRequest>();
        _request_size -= file_request.get_content_length();
        _file_storage.write(file_request.batch.data(), file_request.batch_size);

        if (_request_size == 0)
        {
            break;
        }
    }

    _file_storage.close();
}

boost::asio::awaitable<void> FileController::write_confirmation(Tcp::header_t confirmation_id)
{
    co_await _tcp_writer.write_async(confirmation_id);
}
