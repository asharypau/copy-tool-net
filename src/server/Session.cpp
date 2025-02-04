#include "Session.h"

#include <cstddef>

#include "../utils/Logger.h"
#include "FileWriter.h"
#include "boost/asio/read.hpp"

Session::Session(boost::asio::ip::tcp::socket socket, size_t client_id)
    : _socket(std::move(socket)),
      _file_name(),
      _batch_data(),
      _file(),
      _client_id(client_id),
      _file_size(0),
      _file_name_size(0),
      _batch_size(0)
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

            read_file_name_size();
        });
}

void Session::read_file_name_size()
{
    _file_name_size = 0;

    std::shared_ptr<Session> self = shared_from_this();
    boost::asio::async_read(
        _socket,
        boost::asio::buffer(&_file_name_size, sizeof(_file_name_size)),
        [self, this](const boost::system::error_code& error, size_t read_bytes)
        {
            if (error)
            {
                handle_error(error);
                return;
            }

            read_file_name();
        });
}

void Session::read_file_name()
{
    if (_file_name.size() != _file_name_size)
    {
        _file_name.resize(_file_name_size);
    }

    std::shared_ptr<Session> self = shared_from_this();
    boost::asio::async_read(
        _socket,
        boost::asio::buffer(_file_name.data(), _file_name_size),
        [self, this](const boost::system::error_code& error, size_t read_bytes)
        {
            if (error)
            {
                handle_error(error);
                return;
            }

            _file.create(_file_name);
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
