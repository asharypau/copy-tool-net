#include "Session.h"

#include <cstddef>
#include <memory>
#include <utility>

#include "../utils/Logger.h"
#include "FileWriter.h"

Session::Session(boost::asio::ip::tcp::socket socket, size_t client_id)
    : _client_id(client_id),
      _socket(std::move(socket)),
      _file_size(0),
      _file(),
      _batch_size(0),
      _batch()

{
}

void Session::start()
{
    Logger::info("Client connected: " + std::to_string(_client_id));

    read_file_size();
}

void Session::read_file_size()
{
    _file_size = 0;

    read_async(
        &_file_size,
        sizeof(_file_size),
        [this]
        {
            read_file_name_size();
        });
}

void Session::read_file_name_size()
{
    std::shared_ptr<size_t> file_name_size = std::make_shared<size_t>(0);

    read_async(
        file_name_size.get(),
        sizeof(*file_name_size),
        [this, file_name_size]
        {
            read_file_name(*file_name_size);
        });
}

void Session::read_file_name(size_t file_name_size)
{
    std::shared_ptr<std::string> file_name = std::make_shared<std::string>();
    file_name->resize(file_name_size);

    read_async(
        file_name.get()->data(),
        file_name_size,
        [this, file_name]
        {
            size_t users = file_name.use_count();
            _file.create(*file_name);
            read_batch_size();
        });
}

void Session::read_batch_size()
{
    _batch_size = 0;

    read_async(
        &_batch_size,
        sizeof(_batch_size),
        [this]
        {
            read_batch();
        });
}

void Session::read_batch()
{
    if (_batch.size() != _batch_size)
    {
        _batch.resize(_batch_size);
    }

    read_async(
        _batch.data(),
        _batch_size,
        [this]
        {
            _file.write(_batch.data(), _batch_size);
            _file_size -= _batch_size;

            if (_file_size > 0)
            {
                read_batch_size();
            }
            else
            {
                _file.close();
                read_file_size();
            }
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
