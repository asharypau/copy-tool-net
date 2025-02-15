#ifndef FILE_CLIENT_H
#define FILE_CLIENT_H

#include <functional>
#include <optional>

#include "../common/FileService.h"
#include "../common/Tcp.h"
#include "../models/Message.h"

namespace Client
{
    class FileClient
    {
    public:
        explicit FileClient(boost::asio::ip::tcp::socket& socket);

        /**
         * @brief Writes message via the TCP writer.
         */
        void write(Message message);

        /**
         * @brief Reads confirmations of sending messages via the TCP reader.
         */
        void read();

        /**
         * @brief Registers a callback handler for writing data.
         *
         * @param write_handle A `std::function` representing the callback to be invoked when data is written.
         */
        void register_write_handler(std::function<void()> write_handle)
        {
            _write_handle.emplace(write_handle);
        }

        /**
         * @brief Registers a callback handler for reading data.
         *
         * @param read_handle A `std::function` representing the callback to be invoked when data is read.
         */
        void register_read_handler(std::function<void(size_t)> read_handle)
        {
            _read_handle.emplace(read_handle);
        }

    private:
        /**
         * @brief Prepares the headers and writes their asynchronously via the TCP writer.
         *
         * The header format is as follows:
         * - 1st block: File id (`message.id`).
         * - 2nd block: File name size (`file_name_size`).
         * - 3rd block: File name (`message.name`).
         *
         * @param message The message containing the file information to be sent.
         *
         * The function operates as follows:
         * - Resizes the header buffer based on the file information.
         * - Copies the file id, name size and name into the header buffer.
         * - Asynchronously writes the headers over TCP.
         * - Once the headers are written, it calls `write_file` to transmit the file data.
         */
        void write_headers(Message message);

        /**
         * @brief Reads data from a file and writes it asynchronously via the TCP writer.
         *
         * @param file A unique pointer to the `FileHandler` containing the file to be read and sent.
         *
         * The function operates as follows:
         * - Reads up to `BATCH_SIZE` bytes from the file.
         * - If data was read -> send the data over TCP.
         * - Recursively calls itself upon successful transmission to continue sending the file.
         * - If no data remains, it calls callback (`_write_handle`).
         */
        void write_file();

        static constexpr size_t BATCH_SIZE = 1024 * 1024;

        Tcp::Writer _tcp_writer;
        Tcp::Reader _tcp_reader;
        FileService _file_service;
        std::optional<std::function<void()>> _write_handle;
        std::optional<std::function<void(size_t)>> _read_handle;
    };

}
#endif  // FILE_CLIENT_H
