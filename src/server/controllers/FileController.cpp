#include "FileController.h"

#include <string>

#include "../models/FileHeaders.h"
#include "../models/FileRequest.h"
#include "Startup.h"

using namespace Server;

FileController::FileController(std::string& client_id, Tcp::Reader& tcp_reader, Tcp::Writer& tcp_writer)
    : _client_id(client_id),
      _tcp_reader(tcp_reader),
      _tcp_writer(tcp_writer),
      _file_service(),
      _request_size(0)
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
    _tcp_reader.read_async<FileHeaders>(
        [this, session](FileHeaders file_headers)
        {
            _request_size -= file_headers.get_content_length();
            _file_service.open_create(std::string(Server::CLIENT_STORAGE_PATH) + _client_id, file_headers.name);

            read_file(file_headers.confirmation_id, session);
        });
}

void FileController::read_file(Tcp::header_type confirmation_id, std::shared_ptr<Session> session)
{
    _tcp_reader.read_async<FileRequest>(
        [this, confirmation_id, session](FileRequest file_request)
        {
            _request_size -= file_request.get_content_length();
            _file_service.write(file_request.batch.data(), file_request.batch_size);

            if (_request_size > 0)
            {
                read_file(confirmation_id, session);
            }
            else
            {
                _file_service.close();
                write_confirmation(confirmation_id, session);
            }
        });
}

void FileController::write_confirmation(Tcp::header_type confirmation_id, std::shared_ptr<Session> session)
{
    _tcp_writer.write_async(
        confirmation_id,
        [session]
        {
            session->notify_done();
        });
}
