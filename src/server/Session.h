#ifndef SERVER_SESSION_H
#define SERVER_SESSION_H

#include <boost/asio.hpp>
#include <cstddef>
#include <memory>
#include <vector>

#include "../common/Tcp.h"
#include "FileHandler.h"

namespace Server
{
    class Session : public std::enable_shared_from_this<Session>
    {
    public:
        Session(boost::asio::ip::tcp::socket socket, size_t client_id);

        /**
         * @brief Runs a new session.
         */
        void run();

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
         * - Uses `shared_from_this()` to extend the lifetime of the session during asynchronous operations.
         * - Calls `_tcp_reader.read_async` to asynchronously read the fixed-length header section.
         * - Extracts data from the buffer.
         * - Calls `read_file` to process the received file.
         */
        void read_headers();

        /**
         * @brief Reads and processes file data from the TCP stream.
         *
         * The function operates as follows:
         * - Uses `shared_from_this()` to extend the lifetime of the session during asynchronous operations.
         * - Asynchronously reads a `Tcp::HEADER_SIZE`-sized batch header to determine the next data chunk size.
         * - Extracts the batch size and resizes the buffer if necessary.
         * - Reads the actual file data and writes it to the file.
         * - If more data remains, recursively calls `read_file` to continue reading.
         * - Once the file transfer is complete, sends a confirmation and begins processing the next message.
         *
         * @param file A unique pointer to a `FileHandler` instance that manages file writing.
         */
        void read_file(std::unique_ptr<FileHandler>&& file);

        /**
         * @brief Writes a confirmation via the TCP writer.
         */
        void write_confirmation(std::shared_ptr<FileHandler>&& file);

        boost::asio::ip::tcp::socket _socket;
        Tcp::Reader _tcp_reader;
        Tcp::Writer _tcp_writer;
        std::vector<char> _batch;
        size_t _client_id;
    };
}

#endif  // SERVER_SESSION_H
