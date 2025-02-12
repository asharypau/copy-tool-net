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
      _file_name_buffer(),
      _data_buffer(),
      _file_id(0),
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
            _tcp_reader.extract(&_file_id, Tcp::HEADER_SIZE);

            size_t file_size = 0;
            _tcp_reader.extract(&file_size, Tcp::HEADER_SIZE);

            size_t file_name_size = 0;
            _tcp_reader.extract(&file_name_size, Tcp::HEADER_SIZE);

            if (_file_name_buffer.size() < file_name_size)
            {
                _file_name_buffer.resize(file_name_size);
            }

            _tcp_reader.read(file_name_size);
            _tcp_reader.extract(_file_name_buffer.data(), file_name_size);

            read_file(std::make_unique<FileHandler>(file_size, _file_name_buffer));
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

            if (_data_buffer.size() < batch_size)
            {
                _data_buffer.resize(batch_size);
            }

            _tcp_reader.read(batch_size);
            _tcp_reader.extract(_data_buffer.data(), batch_size);
            file->write(_data_buffer.data(), batch_size);

            if (file->get_bytes_to_write() > 0)
            {
                read_file(std::move(file));
            }
            else
            {
                write_confirmation();
                read_headers();
            }
        });
}

void Session::write_confirmation()
{
    std::shared_ptr<Session> self = shared_from_this();
    _tcp_writer.write(&_file_id, Tcp::HEADER_SIZE, [self] {});
}
