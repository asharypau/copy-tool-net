#include "TcpReader.h"

TcpReader::TcpReader(boost::asio::ip::tcp::socket& socket)
    : _socket(socket),
      _buffer()
{
}

void TcpReader::read(size_t size_in_bytes)
{
    boost::system::error_code error;
    size_t read_bytes = boost::asio::read(_socket, _buffer, boost::asio::transfer_exactly(size_in_bytes), error);

    if (error)
    {
        handle_error(error);
    }
}

void TcpReader::handle_error(const boost::system::error_code& error)
{
    if (error == boost::asio::error::eof)
    {
        Logger::info("Disconnected.");
    }
    else if (error == boost::asio::error::connection_reset)
    {
        Logger::error("The connection was reset by the remote side.");
    }
    else
    {
        Logger::error("An unknown error occurred.");
    }
}
