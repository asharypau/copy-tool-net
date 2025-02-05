#include "Session.h"

Session::Session(boost::asio::ip::tcp::socket socket, size_t client_id)
    : _client_id(client_id),
      _socket(std::move(socket))
{
}

void Session::start()
{
    Logger::info("Client connected: " + std::to_string(_client_id));

    read_file_size();
}

void Session::read_file_size()
{
    std::shared_ptr<size_t> file_size = std::make_shared<size_t>(0);

    read_async(
        file_size.get(),
        sizeof(*file_size),
        [this, file_size]
        {
            std::shared_ptr<FileHandler> file = std::make_shared<FileHandler>(*file_size);

            read_file_name(file);
        });
}

void Session::read_file_name(std::shared_ptr<FileHandler> file)
{
    std::shared_ptr<size_t> file_name_size = std::make_shared<size_t>(0);

    read_async(
        file_name_size.get(),
        sizeof(*file_name_size),
        [this, file, file_name_size]
        {
            std::shared_ptr<std::string> file_name = std::make_shared<std::string>(*file_name_size, '\0');

            read_async(
                file_name.get()->data(),
                *file_name_size,
                [this, file, file_name]
                {
                    file->create(*file_name);
                    read_batch(file);
                });
        });
}

void Session::read_batch(std::shared_ptr<FileHandler> file)
{
    std::shared_ptr<size_t> batch_size = std::make_shared<size_t>(0);

    read_async(
        batch_size.get(),
        sizeof(*batch_size),
        [this, file, batch_size]
        {
            std::shared_ptr<std::vector<char>> batch = std::make_shared<std::vector<char>>(*batch_size);

            read_async(
                batch.get()->data(),
                *batch_size,
                [this, file, batch_size, batch]
                {
                    file->write(batch.get()->data(), *batch_size);

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

void Session::handle_error(const boost::system::error_code& error)
{
    if (error == boost::asio::error::eof)
    {
        Logger::info("Client disconnected: " + std::to_string(_client_id));
    }

    if (error == boost::asio::error::connection_reset)
    {
        Logger::error("The connection was reset by the remote side: " + std::to_string(_client_id));
    }
}
