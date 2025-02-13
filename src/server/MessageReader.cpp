#include "MessageReader.h"

using namespace Server;

MessageReader::MessageReader(boost::asio::ip::tcp::socket& socket)
    : _tcp_reader(socket),
      _batch()
{
}
