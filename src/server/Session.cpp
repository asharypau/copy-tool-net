#include "Session.h"

#include <memory>
#include <string>
#include <utility>
#include <vector>

using namespace Server;

Session::Session(boost::asio::ip::tcp::socket socket, size_t client_id)
    : _socket(std::move(socket)),
      _tcp_reader(_socket),
      _tcp_writer(_socket),
      _batch(),
      _client_id(client_id)
{
}

void Session::run()
{
    Logger::info("Client connected: " + std::to_string(_client_id));

    read_headers();
}

void Session::read_headers()
{
    std::shared_ptr<Session> self = shared_from_this();

    _tcp_reader.read_async(
        Tcp::HEADER_SIZE * 3,
        [this, self]
        {
            size_t file_id = 0;
            _tcp_reader.extract(&file_id, Tcp::HEADER_SIZE);

            size_t file_size = 0;
            _tcp_reader.extract(&file_size, Tcp::HEADER_SIZE);

            size_t file_name_size = 0;
            _tcp_reader.extract(&file_name_size, Tcp::HEADER_SIZE);

            std::string file_name(file_name_size, '\0');
            _tcp_reader.read(file_name_size);
            _tcp_reader.extract(file_name.data(), file_name_size);

            read_file(std::make_unique<FileHandler>(file_id, std::to_string(_client_id), file_name, file_size));
        });
}

void Session::read_file(std::unique_ptr<FileHandler>&& file)
{
    std::shared_ptr<Session> self = shared_from_this();

    _tcp_reader.read_async(
        Tcp::HEADER_SIZE,
        [this, self, file = std::move(file)]() mutable
        {
            size_t batch_size = 0;
            _tcp_reader.extract(&batch_size, Tcp::HEADER_SIZE);

            if (_batch.size() < batch_size)
            {
                _batch.resize(batch_size);
            }

            _tcp_reader.read(batch_size);
            _tcp_reader.extract(_batch.data(), batch_size);
            file->write(_batch.data(), batch_size);

            if (file->get_bytes_to_write() > 0)
            {
                read_file(std::move(file));
            }
            else
            {
                write_confirmation(std::move(file));
                read_headers();
            }
        });
}

void Session::write_confirmation(std::shared_ptr<FileHandler>&& file)
{
    _tcp_writer.write(file->get_id(), Tcp::HEADER_SIZE, [file] {});
}
