#include "FileController.h"

using namespace Server;

FileController::FileController(std::string& client_id, Tcp::Reader& tcp_reader, Tcp::Writer& tcp_writer)
    : _client_id(client_id),
      _tcp_reader(tcp_reader),
      _tcp_writer(tcp_writer),
      _batch(),
      _request_size(0),
      _confirmation_id(0)
{
}

void FileController::handle(size_t request_size, std::shared_ptr<Session> session)
{
    if (request_size != 0)
    {
        _request_size = request_size;
        read_headers(session);
    }
}

void FileController::read_headers(std::shared_ptr<Session> session)
{
    _tcp_reader.read_async(
        Tcp::HEADER_SIZE * 2,
        [this, session]() mutable
        {
            _tcp_reader.extract(&_confirmation_id, Tcp::HEADER_SIZE);

            size_t file_name_size = 0;
            _tcp_reader.extract(&file_name_size, Tcp::HEADER_SIZE);

            std::string file_name(file_name_size, '\0');
            _tcp_reader.read(file_name_size);
            _tcp_reader.extract(file_name.data(), file_name_size);

            _request_size -= Tcp::HEADER_SIZE * 2;
            _request_size -= file_name_size;

            read_file(std::make_unique<FileHandler>(_client_id, file_name), session);
        });
}

void FileController::read_file(std::unique_ptr<FileHandler>&& file, std::shared_ptr<Session> session)
{
    _tcp_reader.read_async(
        Tcp::HEADER_SIZE,
        [this, file = std::move(file), session]() mutable
        {
            size_t batch_size = 0;
            _tcp_reader.extract(&batch_size, Tcp::HEADER_SIZE);

            if (_batch.size() < batch_size)
            {
                _batch.resize(batch_size);
            }

            _tcp_reader.read(batch_size);
            _tcp_reader.extract(_batch.data(), batch_size);

            _request_size -= Tcp::HEADER_SIZE;
            _request_size -= batch_size;

            file->write(_batch.data(), batch_size);

            if (_request_size > 0)
            {
                read_file(std::move(file), session);
            }
            else
            {
                write_confirmation(session);
            }
        });
}

void FileController::write_confirmation(std::shared_ptr<Session> session)
{
    _tcp_writer.write(
        &_confirmation_id,
        Tcp::HEADER_SIZE,
        [session]
        {
            session->notify_done();
        });
}
