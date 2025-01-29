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
            read_file();
        }
        catch (const DisconnectException& ex)
        {
            Logger::info(ex.what());
            break;
        }
        catch (const std::runtime_error& ex)
        {
            Logger::error("Error occurred: " + std::string(ex.what()) + " during processing client: " + std::to_string(_client_id));
            break;
        }
    }
}

void Session::read_file()
{
    size_t file_size = read_size();
    FileHandler file_handler("D:/Education/CppMentoring/files");

    while (file_size > 0)
    {
        size_t batch_size = read_size();
        std::vector<char> batch = read_batch(batch_size);

        file_handler.write(batch.data(), batch_size);

        file_size -= batch_size;
    }
}

size_t Session::read_size()
{
    size_t size = 0;
    read(&size, sizeof(size));

    return size;
}

std::vector<char> Session::read_batch(size_t batch_size)
{
    std::vector<char> buffer(batch_size);
    read(buffer.data(), batch_size);

    return std::move(buffer);
}
