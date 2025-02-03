#include "Client.h"

#include "../exceptions/DisconnectException.h"
#include "../utils/Logger.h"

Client::Client(unsigned short port, const std::string& host)
    : _tcp_client(),
      _file_path_prompt()
{
    _tcp_client.connect(port, host);

    Logger::info("Connected to server by " + host + ":" + std::to_string(port));
}

void Client::run()
{
    try
    {
        write();
    }
    catch (const DisconnectException& ex)
    {
        Logger::info(std::string(ex.what()) + ". Server disconnected");
    }
    catch (const std::exception& ex)
    {
        Logger::error("Error occurred: " + std::string(ex.what()));
    }
}

void Client::write()
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
            FileReader file(file_name);

            size_t file_size = file.get_size();
            _tcp_client.write(&file_size, sizeof(file_size));

            write_file(file);

            Logger::info("File sent: " + file_name + " (" + std::to_string(file_size) + " bytes)");
        }
    }
}

void Client::write_file(FileReader& file)
{
    size_t batch_size = 1024 * 1024;
    size_t buffer_offset = sizeof(batch_size);

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
