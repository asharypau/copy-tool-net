#include "ClientSession.h"

#include "../exceptions/DisconnectException.h"
#include "../utils/Logger.h"
#include "FileReader.h"

ClientSession::ClientSession(TcpClient tcp_client)
    : _tcp_client(std::move(tcp_client)), _file_path_prompt()
{
}

void ClientSession::start()
{
    Logger::info("Connected to server");

    try
    {
        write();
    }
    catch (const DisconnectException& ex)
    {
        Logger::info(std::string(ex.what()) + ". Server disconnected");
    }
    catch (const std::runtime_error& ex)
    {
        Logger::error("Error occurred: " + std::string(ex.what()));
    }
}

void ClientSession::write()
{
    while (true)
    {
        std::vector<std::string> file_names = _file_path_prompt.get();

        if (file_names.empty())
        {
            break;
        }

        for (const std::string& file_name : file_names)
        {
            write_file(file_name);
        }
    }
}

void ClientSession::write_file(const std::string& file_name)
{
    FileReader file(file_name);

    size_t file_size = file.get_size();
    _tcp_client.write(&file_size, sizeof(file_size));

    size_t buffer_offset = sizeof(size_t);
    size_t batch_size = 1024 * 1024;
    std::vector<char> buffer(buffer_offset + batch_size);

    while (true)
    {
        size_t bytes_read = file.read(&buffer[buffer_offset], batch_size);
        if (bytes_read == 0)
        {
            break;
        }

        std::memcpy(buffer.data(), &bytes_read, buffer_offset);

        _tcp_client.write(buffer.data(), buffer_offset + bytes_read);
    }
}
