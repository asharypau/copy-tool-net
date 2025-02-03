#include "Session.h"

#include <cstddef>

#include "../utils/Logger.h"
#include "FileWriter.h"

Session::Session(boost::asio::ip::tcp::socket socket, size_t client_id)
    : _socket(std::move(socket)),
      _batch_data(),
      _file(),
      _client_id(client_id),
      _file_size(0),
      _batch_size(0)
{
}

void Session::start()
{
    Logger::info("Client connected: " + std::to_string(_client_id));

    read();
}

void Session::read()
{
    _file_size = 0;

    std::shared_ptr<Session> self = shared_from_this();
    boost::asio::async_read(
        _socket,
        boost::asio::buffer(&_file_size, sizeof(_file_size)),
        [self, this](const boost::system::error_code& error, size_t read_bytes)
        {
            if (error)
            {
                handle_error(error);
                return;
            }

            read_batch_size();
        });
}

void Session::read_batch_size()
{
    _batch_size = 0;

    std::shared_ptr<Session> self = shared_from_this();
    boost::asio::async_read(
        _socket,
        boost::asio::buffer(&_batch_size, sizeof(_batch_size)),
        [self, this](const boost::system::error_code& error, size_t read_bytes)
        {
            if (error)
            {
                handle_error(error);
                return;
            }

            if (!_file.is_open())
            {
                _file.open("sad");
            }

            read_batch();
        });
}

void Session::read_batch()
{
    if (_batch_data.size() != _batch_size)
    {
        _batch_data.resize(_batch_size);
    }

    std::shared_ptr<Session> self = shared_from_this();
    boost::asio::async_read(
        _socket,
        boost::asio::buffer(_batch_data.data(), _batch_size),
        [self, this](const boost::system::error_code& error, size_t read_bytes)
        {
            if (error)
            {
                handle_error(error);
                return;
            }

            _file.write(_batch_data.data(), _batch_size);
            _file_size -= _batch_size;

            if (_file_size > 0)
            {
                read_batch_size();
            }
            else
            {
                _file.close();
                read();
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
