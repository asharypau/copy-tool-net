#include "TcpWriter.h"

TcpWriter::TcpWriter(boost::asio::ip::tcp::socket& socket)
    : _socket(socket)
{
}
