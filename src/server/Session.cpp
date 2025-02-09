#include "Session.h"

#include <memory>
#include <string>
#include <utility>
#include <vector>

Session::Session(boost::asio::ip::tcp::socket socket, size_t client_id)
    : _socket(std::move(socket)),
      _tcp_reader(_socket),
      _client_id(client_id)
{
}

void Session::run()
{
    Logger::info("Client connected: " + std::to_string(_client_id));

    get_headers();
}

void Session::get_headers()
{
    std::shared_ptr<Session> self = shared_from_this();

    _tcp_reader.read_async(
        HEADER_SIZE,
        [this, self]
        {
            size_t file_size = 0;
            _tcp_reader.extract(&file_size, HEADER_SIZE);

            _tcp_reader.read(HEADER_SIZE);
            size_t file_name_size = 0;
            _tcp_reader.extract(&file_name_size, HEADER_SIZE);

            _tcp_reader.read(file_name_size);
            std::string file_name(file_name_size, '\0');
            _tcp_reader.extract(file_name.data(), file_name_size);

            get_file(std::make_unique<FileHandler>(file_size, file_name));
        });
}

void Session::get_file(std::unique_ptr<FileHandler>&& file)
{
    std::shared_ptr<Session> self = shared_from_this();

    _tcp_reader.read_async(
        HEADER_SIZE,
        [this, self, file = std::move(file)]() mutable
        {
            size_t batch_size = 0;
            _tcp_reader.extract(&batch_size, HEADER_SIZE);

            _tcp_reader.read_async(
                batch_size,
                [this, self, file = std::move(file), batch_size]() mutable
                {
                    std::vector<char> data(batch_size);
                    _tcp_reader.extract(data.data(), batch_size);
                    file->write(data.data(), batch_size);

                    if (file->get_bytes_to_write() > 0)
                    {
                        get_file(std::move(file));
                    }
                    else
                    {
                        get_headers();
                    }
                });
        });
}
