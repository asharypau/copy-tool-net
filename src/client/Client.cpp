#include "Client.h"

#include "../utils/Logger.h"

Client::Client(unsigned short port, std::string host)
    : _tcp_client(),
      _file_path_prompt(),
      _host(host),
      _port(port)
{
}

void Client::run()
{
    try
    {
        Logger::info("Client started");

        _tcp_client.connect(_port, _host);

        Logger::info("Connected to server by " + _host + ":" + std::to_string(_port));

        write();
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
        std::vector<std::pair<std::string, std::string>> files_data = _file_path_prompt.get();
        if (files_data.empty())
        {
            break;
        }

        for (const std::pair<std::string, std::string>& file_data : files_data)
        {
            FileReader file(file_data.first, file_data.second);

            write_file_size(file.get_size());
            write_file_name(file.get_name());
            write_file(file);

            Logger::info("File sent: " + file.get_name() + " (" + std::to_string(file.get_size()) + " bytes)");
        }
    }
}

void Client::write_file_size(size_t size)
{
    _tcp_client.write(&size, sizeof(size));
}

void Client::write_file_name(const std::string& name)
{
    size_t name_size = name.size();
    size_t buffer_offset = sizeof(name_size);

    std::vector<char> buffer(buffer_offset + name_size);

    std::memcpy(buffer.data(), &name_size, buffer_offset);        // write a size of name into the buffer
    std::memcpy(&buffer[buffer_offset], name.data(), name_size);  // write a name into the buffer

    _tcp_client.write(buffer.data(), buffer_offset + name_size);
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

        std::memcpy(buffer.data(), &bytes_read, buffer_offset);  // write batch size into the buffer

        _tcp_client.write(buffer.data(), buffer_offset + bytes_read);
    }
}
