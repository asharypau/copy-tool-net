#include "MessageHandler.h"

using namespace Client;

MessageHandler::MessageHandler(boost::asio::ip::tcp::socket& socket)
    : _headers(),
      _batch(Tcp::HEADER_SIZE + BATCH_SIZE),
      _tcp_writer(socket),
      _tcp_reader(socket),
      _write_handle(),
      _read_handle()
{
}

void MessageHandler::write(Message message)
{
    write_headers(message);
}

void MessageHandler::read()
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

void MessageHandler::register_write_handler(std::function<void()> write_handle)
{
    _write_handle.emplace(write_handle);
}

void MessageHandler::register_read_handler(std::function<void(size_t)> read_handle)
{
    _read_handle.emplace(read_handle);
}

void MessageHandler::write_headers(Message message)
{
    size_t file_name_size = message.name.size();

    _headers.resize(Tcp::HEADER_SIZE * 3 + file_name_size);
    std::memcpy(_headers.data(), &message.id, Tcp::HEADER_SIZE);                               // write a file id into the buffer at index 0
    std::memcpy(_headers.data() + Tcp::HEADER_SIZE, &message.size, Tcp::HEADER_SIZE);          // write a file size into the buffer at index Tcp::HEADER_SIZE
    std::memcpy(_headers.data() + Tcp::HEADER_SIZE * 2, &file_name_size, Tcp::HEADER_SIZE);    // write a file name size into the buffer at index 0 + Tcp::HEADER_SIZE * 2
    std::memcpy(_headers.data() + Tcp::HEADER_SIZE * 3, message.name.data(), file_name_size);  // write a name into the buffer at index 0 + Tcp::HEADER_SIZE * 3

    _tcp_writer.write(
        _headers.data(),
        _headers.size(),
        [this, message]
        {
            write_file(std::make_unique<FileHandler>(message.path));
        });
}

void MessageHandler::write_file(std::unique_ptr<FileHandler>&& file)
{
    size_t batch_size = file->read(_batch.data() + Tcp::HEADER_SIZE, BATCH_SIZE);  // write data into the buffer at index 0 + SIZE
    if (batch_size > 0)
    {
        std::memcpy(_batch.data(), &batch_size, Tcp::HEADER_SIZE);  // write batch size into the buffer at index 0

        _tcp_writer.write(
            _batch.data(),
            Tcp::HEADER_SIZE + batch_size,
            [this, file = std::move(file)]() mutable
            {
                write_file(std::move(file));
            });
    }
    else
    {
        if (_write_handle.has_value())
        {
            _write_handle.value()();
        }
    }
}
