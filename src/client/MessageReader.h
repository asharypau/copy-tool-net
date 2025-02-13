#ifndef MESSAGE_READER_H
#define MESSAGE_READER_H

#include <functional>
#include <optional>

#include "../common/Tcp.h"

namespace Client
{
    class MessageReader
    {
    public:
        explicit MessageReader(boost::asio::ip::tcp::socket& socket);

        /**
         * @brief Reads confirmations of sending messages via the TCP reader.
         *
         * The flow of the function is as follows:
         * - Once the confirmation is received, the file id is extracted from the data.
         * - If a read handler is registered, the handler is invoked with the extracted id.
         */
        void read();

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
        Tcp::Reader _tcp_reader;
        std::optional<std::function<void(size_t)>> _read_handle;
    };
}

#endif  // MESSAGE_READER_H
