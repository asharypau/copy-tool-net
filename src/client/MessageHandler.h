#ifndef MESSAGE_WRITER_H
#define MESSAGE_WRITER_H

#include <functional>
#include <memory>
#include <optional>
#include <vector>

#include "../common/Tcp.h"
#include "../models/Message.h"
#include "FileHandler.h"

namespace Client
{
    class MessageHandler
    {
    public:
        explicit MessageHandler(boost::asio::ip::tcp::socket& socket);

        void write(Message message);
        void read();
        void register_write_handler(std::function<void()> write_handle);
        void register_read_handler(std::function<void(size_t)> read_handle);

    private:
        void write_headers(Message message);
        void write_file(std::unique_ptr<FileHandler>&& file);

        static constexpr size_t BATCH_SIZE = 1024 * 1024;

        std::vector<char> _header_buffer;
        std::vector<char> _data_buffer;
        Tcp::Writer _tcp_writer;
        Tcp::Reader _tcp_reader;
        std::optional<std::function<void()>> _write_handle;
        std::optional<std::function<void(size_t)>> _read_handle;
    };
}

#endif  // MESSAGE_WRITER_H
