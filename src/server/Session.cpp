#include "Session.h"

#include <cstddef>
#include <string>

Session::Session(boost::asio::ip::tcp::socket socket, size_t client_id)
    : _client_id(client_id),
      _socket(std::move(socket)),
      _buffer()
{
}

void Session::run()
{
    Logger::info("Client connected: " + std::to_string(_client_id));

    read_file_size();
}

void Session::read_file_size()
{
    get_size(
        [this]
        {
            size_t size = read_size_from_buffer();
            std::shared_ptr<FileHandler> file = std::make_shared<FileHandler>(size);

            read_file_name(file);
        });
}

void Session::read_file_name(std::shared_ptr<FileHandler> file)
{
    get_size(
        [this, file]
        {
            size_t size = read_size_from_buffer();

            get_data(
                size,
                [this, file, size]
                {
                    std::istreambuf_iterator<char> start(&_buffer);
                    std::istreambuf_iterator<char> end;

                    file->create(std::string(start, end));

                    read_batch(file);
                });
        });
}

void Session::read_batch(std::shared_ptr<FileHandler> file)
{
    get_size(
        [this, file]
        {
            size_t size = read_size_from_buffer();

            get_data(
                size,
                [this, file, size]
                {
                    file->write(_buffer, size);
                    if (file->get_bytes_to_write() > 0)
                    {
                        read_batch(file);
                    }
                    else
                    {
                        read_file_size();
                    }
                });
        });
}

size_t Session::read_size_from_buffer()
{
    const size_t* raw_data = boost::asio::buffer_cast<const size_t*>(_buffer.data());
    _buffer.consume(sizeof(*raw_data));

    return *raw_data;
}

void Session::handle_error(const boost::system::error_code& error)
{
    if (error == boost::asio::error::eof)
    {
        Logger::info("Client disconnected: " + std::to_string(_client_id));
    }
    else if (error == boost::asio::error::connection_reset)
    {
        Logger::error("The connection was reset by the remote side: " + std::to_string(_client_id));
    }
    else
    {
        Logger::error("An unknown error occurred: " + std::to_string(_client_id));
    }
}
