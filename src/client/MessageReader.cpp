#include "MessageReader.h"

using namespace Client;

MessageReader::MessageReader(boost::asio::ip::tcp::socket& socket)
    : _tcp_reader(socket),
      _read_handle()
{
}

void MessageReader::read()
{
    _tcp_reader.read_async(
        Tcp::HEADER_SIZE,
        [this]
        {
            size_t id;
            _tcp_reader.extract(&id, Tcp::HEADER_SIZE);

            if (_read_handle.has_value())
            {
                _read_handle.value()(id);
            }
        });
}
