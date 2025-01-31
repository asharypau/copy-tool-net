#include <string>

#include "../exceptions/DisconnectException.h"
#include "../utils/Logger.h"
#include "FileWriter.h"
#include "Session.h"


Session::Session(TcpClient tcp_client, size_t client_id)
    : _tcp_client(std::move(tcp_client)), _client_id(client_id)
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
            Logger::info(std::string(ex.what()) + ". Disconnected client: " + std::to_string(_client_id));
            break;
        }
        catch (const std::runtime_error& ex)
        {
            Logger::error("Error occurred: " + std::string(ex.what()) + " during processing client: " + std::to_string(_client_id));
            break;
        }
    }
}

void Session::read()
{
    size_t file_size = read_size();
    FileWriter file("D:/Education/CppMentoring/files");

    while (file_size > 0)
    {
        size_t batch_size = read_size();
        std::vector<char> batch = read_batch(batch_size);

        file.write(batch.data(), batch_size);

        file_size -= batch_size;
    }
}

size_t Session::read_size()
{
    size_t size = 0;
    _tcp_client.read(&size, sizeof(size));

    return size;
}

std::vector<char> Session::read_batch(size_t batch_size)
{
    std::vector<char> buffer(batch_size);
    _tcp_client.read(buffer.data(), batch_size);

    return std::move(buffer);
}
