#ifndef MESSAGE_READER_H
#define MESSAGE_READER_H

#include <boost/asio.hpp>
#include <memory>
#include <vector>

#include "../common/Tcp.h"
#include "FileHandler.h"

namespace Server
{
    class MessageReader
    {
    public:
        explicit MessageReader(boost::asio::ip::tcp::socket& socket);

        /**
         * @brief Reads message via the TCP reader.
         *
         * @param callback The callback function or callable object to be invoked once the read operation completes successfully.
         */
        template <class TCallback>
        void read(const std::string& file_path, TCallback&& callback)
        {
            read_headers(file_path, std::forward<TCallback>(callback));
        }

    private:
        /**
         * @brief Reads and processes the file headers from the TCP stream.
         *
         * This function asynchronously reads the file headers, which include:
         * - File id (size: `Tcp::HEADER_SIZE` bytes)
         * - File size (size: `Tcp::HEADER_SIZE` bytes)
         * - File name size (size: `Tcp::HEADER_SIZE` bytes)
         * - File name (variable length)
         *
         * The function operates as follows:
         * - Calls `_tcp_reader.read_async` to asynchronously read the fixed-length header section.
         * - Extracts data from the buffer.
         * - Calls `read_file` to process the received file.
         *
         * @param callback The callback function or callable object to be invoked once the read operation completes successfully.
         */
        template <class TCallback>
        void read_headers(const std::string& file_path, TCallback&& callback)
        {
            _tcp_reader.read_async(
                Tcp::HEADER_SIZE * 3,
                [this, file_path, callback = std::forward<TCallback>(callback)]() mutable
                {
                    size_t file_id = 0;
                    _tcp_reader.extract(&file_id, Tcp::HEADER_SIZE);

                    size_t file_size = 0;
                    _tcp_reader.extract(&file_size, Tcp::HEADER_SIZE);

                    size_t file_name_size = 0;
                    _tcp_reader.extract(&file_name_size, Tcp::HEADER_SIZE);

                    std::string file_name(file_name_size, '\0');
                    _tcp_reader.read(file_name_size);
                    _tcp_reader.extract(file_name.data(), file_name_size);

                    read_file(std::make_unique<FileHandler>(file_id, file_path, file_name, file_size), std::forward<TCallback>(callback));
                });
        }

        /**
         * @brief Reads and processes file data from the TCP stream.
         *
         * The function operates as follows:
         * - Asynchronously reads a `Tcp::HEADER_SIZE`-sized batch header to determine the next data chunk size.
         * - Extracts the batch size and resizes the buffer if necessary.
         * - Reads the actual file data and writes it to the file.
         * - If more data remains, recursively calls `read_file` to continue reading.
         * - Once the file transfer is complete, sends a confirmation and begins processing the next message.
         *
         * @param file A unique pointer to a `FileHandler` instance that manages file writing.
         * @param callback The callback function or callable object to be invoked once the read operation completes successfully.
         */
        template <class TCallback>
        void read_file(std::unique_ptr<FileHandler>&& file, TCallback&& callback)
        {
            _tcp_reader.read_async(
                Tcp::HEADER_SIZE,
                [this, file = std::move(file), callback = std::forward<TCallback>(callback)]() mutable
                {
                    size_t batch_size = 0;
                    _tcp_reader.extract(&batch_size, Tcp::HEADER_SIZE);

                    if (_batch.size() < batch_size)
                    {
                        _batch.resize(batch_size);
                    }

                    _tcp_reader.read(batch_size);
                    _tcp_reader.extract(_batch.data(), batch_size);
                    file->write(_batch.data(), batch_size);

                    if (file->get_bytes_to_write() > 0)
                    {
                        read_file(std::move(file), std::forward<TCallback>(callback));
                    }
                    else
                    {
                        callback(file->get_id());
                    }
                });
        }

        Tcp::Reader _tcp_reader;
        std::vector<char> _batch;
    };
}

#endif  // MESSAGE_READER_H