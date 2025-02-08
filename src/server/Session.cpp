#include "Session.h"

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

    get_headers();
}

void Session::get_headers()
{
    get_async(
        HEADER_SIZE,
        [this]
        {
            size_t file_size = get_from_buffer<size_t>();

            get(HEADER_SIZE);
            size_t file_name_size = get_from_buffer<size_t>();

            get(file_name_size);
            std::string str(file_name_size, '\0');
            extract(str.data(), file_name_size);

            std::istreambuf_iterator<char> start(&_buffer);
            std::istreambuf_iterator<char> end;
            std::string file_name(start, end);

            std::shared_ptr<FileHandler> file = std::make_shared<FileHandler>(file_size);
            file->create(file_name);

            get_file(file);
        });
}

void Session::get_file(std::shared_ptr<FileHandler> file_handler)
{
    get_async(
        HEADER_SIZE,
        [this, file_handler]
        {
            size_t batch_size = get_from_buffer<size_t>();

            get_async(
                batch_size,
                [this, file_handler, batch_size]
                {
                    file_handler->write(_buffer, batch_size);
                    if (file_handler->get_bytes_to_write() > 0)
                    {
                        get_file(file_handler);
                    }
                    else
                    {
                        get_headers();
                    }
                });
        });
}

void Session::get(size_t size_in_bytes)
{
    boost::system::error_code error;
    size_t read_bytes = boost::asio::read(_socket, _buffer, boost::asio::transfer_exactly(size_in_bytes), error);
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
