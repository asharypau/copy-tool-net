#ifndef FILE_CLIENT_H
#define FILE_CLIENT_H

#include <functional>
#include <optional>

#include "../common/models/Message.h"
#include "../common/network/tcp/Reader.h"
#include "../common/network/tcp/Writer.h"
#include "../common/services/FileService.h"

namespace Client
{
    class FileClient
    {
    public:
        explicit FileClient(boost::asio::ip::tcp::socket& socket);

        /**
         * @brief Writes asynchronously message via the TCP writer.
         *
         * The function operates as follows:
         * - Calculates the number of batches required to send the file message based on the message size and batch size.
         * - Constructs a `RequestMetadata` object, setting the endpoint, size, and other necessary details.
         * - Calls `write_async` to asynchronously send the request metadata.
         * - Upon successful completion of the write operation, triggers the `write_headers` function to send the file headers.
         *
         * @param message The file message to be written asynchronously.
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
        void register_read_handler(std::function<void(Tcp::header_t)> read_handle)
        {
            _read_handle.emplace(read_handle);
        }

    private:
        /**
         * @brief Initiates an asynchronous write operation to send file headers.
         *
         * The function operates as follows:
         * - Constructs `FileHeaders` containing the confirmation ID and file name from the message.
         * - Calls `write_async` to asynchronously send the constructed headers.
         * - Upon successful completion of the write operation, opens the file for reading using `_file_service` and initiates the file writing process.
         *
         * @param message The message containing file details (confirmation ID, file name, and path).
         */
        void write_headers(Message message);

        /**
         * @brief Reads file data in batches and writes it asynchronously to the TCP socket.
         *
         * The function operates as follows:
         * - Reads a batch of data from the file using `_file_service`.
         * - If data is successfully read (batch size > 0), it asynchronously writes the data using `write_async`.
         * - After writing, it recursively calls `write_file` to continue sending the next batch.
         * - If no more data is available, it closes the file and invokes the provided callback (if available) to signal the completion of the write process.
         */
        void write_file();

        static constexpr size_t BATCH_SIZE = 1024 * 1024;

        Tcp::Writer _tcp_writer;
        Tcp::Reader _tcp_reader;
        FileService _file_service;
        std::optional<std::function<void()>> _write_handle;
        std::optional<std::function<void(Tcp::header_t)>> _read_handle;
    };

}
#endif  // FILE_CLIENT_H
