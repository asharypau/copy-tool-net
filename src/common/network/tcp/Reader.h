#ifndef TCP_READER_H
#define TCP_READER_H

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <cstddef>
#include <string>

#include "Constants.h"
#include "Details.h"
#include "OperationException.h"
#include "boost/asio/as_tuple.hpp"

namespace Tcp
{
    class Reader
    {
    public:
        explicit Reader(boost::asio::ip::tcp::socket& socket)
            : _socket(socket),
              _buffer()
        {
        }

        // Disallow copying and assignment.
        Reader(const Reader&) = delete;
        Reader& operator=(const Reader&) = delete;

        // Disallow movement and assignment.
        Reader(Reader&& other) = delete;
        Reader& operator=(Reader&&) = delete;

        template <class TModel>
        boost::asio::awaitable<TModel> read_async()
        {
            Tcp::header_t content_length = co_await read_content_length_async();

            internal_read_data(content_length);
            TModel model;
            extract(&model, sizeof(TModel));

            co_return model;
        }

        template <Tcp::Details::serializable_constraint TSerializableModel>
        boost::asio::awaitable<TSerializableModel> read_async()
        {
            Tcp::header_t content_length = co_await read_content_length_async();

            internal_read_data(content_length);
            TSerializableModel model = extract<TSerializableModel>(content_length);

            co_return model;
        }

    private:
        boost::asio::awaitable<Tcp::header_t> read_content_length_async()
        {
            auto [error, _] = co_await boost::asio::async_read(
                _socket,
                _buffer.prepare(HEADER_SIZE),
                boost::asio::transfer_exactly(HEADER_SIZE),
                boost::asio::as_tuple(boost::asio::use_awaitable));

            if (error)
            {
                throw Tcp::OperationException(error);
            }

            _buffer.commit(HEADER_SIZE);
            Tcp::header_t content_length = 0;
            extract(&content_length, CONTENT_SIZE);

            co_return content_length;
        }

        /**
         * @brief Performs a synchronous read operation on the TCP socket.
         *
         * The function operates as follows:
         * - Calls `boost::asio::read` to read exactly `content_length` bytes from the socket.
         * - If the read operation succeeds, the data is stored in `_buffer`.
         * - If an error occurs, the error is passed to an error handler.
         *
         * @param content_length The exact number of bytes to read from the socket.
         */
        void internal_read_data(size_t content_length)
        {
            boost::system::error_code error;
            boost::asio::read(_socket, _buffer.prepare(content_length), boost::asio::transfer_exactly(content_length), error);

            if (error)
            {
                throw Tcp::OperationException(error);
            }
            else
            {
                _buffer.commit(content_length);
            }
        }

        /**
         * @brief Extracts a specified number of bytes from the internal buffer.
         *
         * The function operates as follows:
         * - Uses `static_cast` to obtain a raw pointer to the buffer data.
         * - Consumes (removes) the extracted bytes from `_buffer`.
         *
         * @tparam TData The type of data being extracted.
         * @param data A pointer to the destination memory where the extracted bytes will be stored.
         * @param size_in_bytes The number of bytes to extract from `_buffer`.
         * @throws std::runtime_error If the buffer does not contain enough data.
         */
        template <class TData>
        void extract(TData* data, size_t size_in_bytes)
        {
            std::byte* begin = static_cast<std::byte*>(_buffer.data().data());
            std::memcpy(data, begin, size_in_bytes);

            _buffer.consume(size_in_bytes);
        }

        /**
         * @brief Extracts and deserializes a model from the internal buffer.
         *
         * The function operates as follows:
         * - Creates an instance of `TSerializableModel` with the specified content length.
         * - Calls the model's `deserialize` method to extract data from the buffer.
         * - Consumes (removes) the extracted bytes from `_buffer`.
         * - Returns the deserialized model.
         *
         * @tparam TSerializableModel The type of the model to extract, constrained to be serializable.
         * @param content_length The expected length of the serialized data in bytes.
         * @return The deserialized `TSerializableModel` instance.
         * @throws std::runtime_error If the buffer does not contain enough data or deserialization fails.
         */
        template <Tcp::Details::serializable_constraint TSerializableModel>
        TSerializableModel extract(Tcp::header_t content_length)
        {
            TSerializableModel model(content_length);
            Tcp::header_t consumed_bytes = model.deserialize(_buffer);
            if (consumed_bytes != content_length)
            {
                std::string message = std::format("Attempt to consume ({}) bytes, although ({}) bytes were read", consumed_bytes, content_length);
                throw std::runtime_error(message);
            }

            _buffer.consume(consumed_bytes);

            return model;
        }

        boost::asio::ip::tcp::socket& _socket;
        boost::beast::flat_buffer _buffer;
    };
}

#endif  // TCP_READER_H
