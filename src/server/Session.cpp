#include "Session.h"

#include "../exceptions/DisconnectException.h"
#include "../utils/Logger.h"
#include "FileHandler.h"

Session::Session(boost::asio::ip::tcp::socket socket, size_t client_id)
    : _socket(std::move(socket)), _client_id(client_id)
{
}

void Session::start()
{
    Logger::info("Client connected: " + std::to_string(_client_id));

    while (true)
    {
        try
        {
            read();
        }
        catch (const DisconnectException& ex)
        {
            Logger::info(ex.what());
            break;
        }
        catch (const std::exception& ex)
        {
            Logger::error("Error occurred: " + std::string(ex.what()) + " during processing client: " + std::to_string(_client_id));
            break;
        }
    }
}

void Session::read()
{
    size_t file_size = read_file_size();
    FileHandler file_handler("D:/Education/CppMentoring/files");

    while (file_size > 0)
    {
        size_t batch_size = read_batch_size();
        std::vector<char> batch = read_batch(batch_size);

        file_handler.write(batch.data(), batch_size);

        file_size -= batch_size;
    }
}

size_t Session::read_file_size()
{
    boost::system::error_code error;
    size_t file_size = 0;
    size_t read_bytes = boost::asio::read(_socket, boost::asio::buffer(&file_size, sizeof(file_size)), error);

    validate_read(error, read_bytes, sizeof(file_size));

    return file_size;
}

size_t Session::read_batch_size()
{
    boost::system::error_code error;
    size_t batch_size = 0;
    size_t read_bytes = boost::asio::read(_socket, boost::asio::buffer(&batch_size, sizeof(batch_size)), error);

    validate_read(error, read_bytes, sizeof(batch_size));

    return batch_size;
}

std::vector<char> Session::read_batch(size_t batch_size)
{
    boost::system::error_code error;
    std::vector<char> buffer(batch_size);
    size_t read_bytes = boost::asio::read(_socket, boost::asio::buffer(buffer.data(), batch_size), error);

    validate_read(error, read_bytes, batch_size);

    return std::move(buffer);
}

void Session::validate_read(boost::system::error_code& error, size_t read_bytes, size_t expected_bytes)
{
    if (error == boost::asio::error::eof || error == boost::asio::error::connection_reset)
    {
        throw DisconnectException("Client disconnected: " + std::to_string(_client_id));
    }

    if (read_bytes != expected_bytes)
    {
        throw std::runtime_error("Unexpected bytes count received: " + std::to_string(read_bytes) + " instead of " + std::to_string(expected_bytes));
    }
}
