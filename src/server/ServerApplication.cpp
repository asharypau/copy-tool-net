#include "ServerApplication.h"

#include <fstream>

#include "../utils/Logger.h"

void ServerApplication::run(CmdArgs cmd_args)
{
    try
    {
        Logger::info("Server started");

        boost::asio::io_context context;

        boost::asio::ip::tcp::socket socket(context);
        boost::asio::ip::tcp::acceptor acceptor(context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), cmd_args.port));

        acceptor.accept(socket);

        read(socket);
    }
    catch (const std::exception& ex)
    {
        Logger::error(ex.what());
    }
}

void ServerApplication::read(boost::asio::ip::tcp::socket& socket)
{
    while (true)
    {
        if (socket.available() == 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        uint64_t file_size = 0;
        socket.read_some(boost::asio::buffer(&file_size, sizeof(file_size)));

        std::ofstream file("../../../files/received_file.txt", std::ios::binary);
        std::vector<char> buffer;

        while (file_size > 0)
        {
            std::streamsize batch_size = 0;
            socket.read_some(boost::asio::buffer(&batch_size, sizeof(std::streamsize)));

            buffer.resize(batch_size);
            socket.read_some(boost::asio::buffer(buffer.data(), batch_size));

            file.write(buffer.data(), batch_size);

            file_size -= batch_size;
        }

        Logger::info("File received");
    }
}
