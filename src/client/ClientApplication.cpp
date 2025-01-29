#include "ClientApplication.h"

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "../utils/Logger.h"

void ClientApplication::run(CmdArgs cmd_args)
{
    try
    {
        Logger::info("Client started");

        boost::asio::io_context context;
        boost::asio::ip::tcp::socket socket(context);

        boost::asio::ip::tcp::resolver resolver(context);
        boost::asio::ip::tcp::resolver::results_type endpoint = resolver.resolve(cmd_args.host, std::to_string(cmd_args.port));

        connect(socket, endpoint);

        Logger::info("Connected to server");

        write(socket);
    }
    catch (const std::exception& ex)
    {
        Logger::error(ex.what());
    }
}

void ClientApplication::write(boost::asio::ip::tcp::socket& socket)
{
    while (true)
    {
        std::vector<std::string> file_names = get_file_names();

        for (const std::string& file_name : file_names)
        {
            write_batch(socket, file_name);
        }
    }
}

void ClientApplication::write_batch(boost::asio::ip::tcp::socket& socket, const std::string& file_name)
{
    if (!std::filesystem::exists(file_name))
    {
        Logger::error("File does not exist: " + file_name);
        return;
    }

    std::ifstream file(file_name, std::ifstream::binary);
    if (!file)
    {
        Logger::error("Failed to open file: " + file_name);
        return;
    }

    uint64_t file_size = std::filesystem::file_size(file_name);
    boost::asio::write(socket, boost::asio::buffer(&file_size, sizeof(file_size)));

    uint64_t buffer_offset = sizeof(std::streamsize);
    std::streamsize batch_size = 1024 * 1024;
    std::vector<char> buffer(buffer_offset + batch_size);

    while (true)
    {
        file.read(&buffer[buffer_offset], batch_size);
        std::streamsize bytes_read = file.gcount();

        if (bytes_read == 0)
        {
            break;
        }

        std::memcpy(buffer.data(), &bytes_read, buffer_offset);

        socket.write_some(boost::asio::buffer(buffer.data(), buffer_offset + bytes_read));
    }
}

std::vector<std::string> ClientApplication::get_file_names()
{
    std::string input;
    std::string file;
    std::vector<std::string> files;

    std::cout << "Enter files to send: ";

    std::getline(std::cin, input);
    std::istringstream stream(input);

    while (stream >> file)
    {
        files.push_back(file);
    }

    return std::move(files);
}
