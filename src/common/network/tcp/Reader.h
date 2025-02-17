#ifndef TCP_READER_H
#define TCP_READER_H

#include <boost/asio.hpp>
#include <string>

#include "../../../utils/Logger.h"
#include "Constants.h"
#include "Utils.h"

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

        template <class TModel, class TCallback>
        void read_async(TCallback&& callback)
        {
            boost::asio::async_read(
                _socket,
                _buffer,
                boost::asio::transfer_exactly(HEADER_SIZE),
                [this, callback = std::forward<TCallback>(callback)](const boost::system::error_code& error, size_t)
                {
                    try
                    {
                        if (error)
                        {
                            Tcp::Utils::handle_error(error);
                        }
                        else
                        {
                            Tcp::header_t content_length = 0;
                            extract(&content_length, CONTENT_SIZE);

                            internal_read(content_length);
                            TModel model;
                            extract(&model, sizeof(TModel));

                            callback(model);
                        }
                    }
                    catch (const std::exception& ex)
                    {
                        Logger::error("Error occurred during read: " + std::string(ex.what()));
                    }
                });
        }

        template <Tcp::Utils::serializable_constraint TSerializableModel, class TCallback>
        void read_async(TCallback&& callback)
        {
            boost::asio::async_read(
                _socket,
                _buffer,
                boost::asio::transfer_exactly(HEADER_SIZE),
                [this, callback = std::forward<TCallback>(callback)](const boost::system::error_code& error, size_t)
                {
                    try
                    {
                        if (error)
                        {
                            Tcp::Utils::handle_error(error);
                        }
                        else
                        {
                            Tcp::header_t content_length = 0;
                            extract(&content_length, CONTENT_SIZE);

                            internal_read(content_length);
                            TSerializableModel model = extract<TSerializableModel>(content_length);

                            callback(model);
                        }
                    }
                    catch (const std::exception& ex)
                    {
                        Logger::error("Error occurred during read: " + std::string(ex.what()));
                    }
                });
        }

    private:
        /**
         * @brief Performs a synchronous read operation on the TCP socket.
         *
         * The function operates as follows:
         * - Calls `boost::asio::read` to read exactly `size_in_bytes` bytes from the socket.
         * - If the read operation succeeds, the data is stored in `_buffer`.
         * - If an error occurs, the error is passed to an error handler.
         *
         * @param size_in_bytes The exact number of bytes to read from the socket.
         */
        void internal_read(size_t size_in_bytes)
        {
            boost::system::error_code error;
            size_t read_bytes = boost::asio::read(_socket, _buffer, boost::asio::transfer_exactly(size_in_bytes), error);

            if (error)
            {
                Tcp::Utils::handle_error(error);
            }
        }

        /**
         * @brief Extracts a specified number of bytes from the internal buffer.
         *
         * The function operates as follows:
         * - Uses `boost::asio::buffer_cast` to obtain a raw pointer to the buffer data.
         * - Consumes (removes) the extracted bytes from `_buffer`.
         *
         * @tparam TData The type of data being extracted.
         * @param data A pointer to the destination memory where the extracted bytes will be stored.
         * @param size_in_bytes The number of bytes to extract from `_buffer`.
         */
        template <class TData>
        void extract(TData* data, size_t size_in_bytes)
        {
            if (_buffer.size() < size_in_bytes)
            {
                throw std::runtime_error("Buffer underflow");
            }

            const TData* raw_data = boost::asio::buffer_cast<const TData*>(_buffer.data());
            std::memcpy(data, raw_data, size_in_bytes);

            _buffer.consume(size_in_bytes);
        }

        template <Tcp::Utils::serializable_constraint TSerializableModel>
        TSerializableModel extract(Tcp::header_t content_length)
        {
            if (_buffer.size() < content_length)
            {
                throw std::runtime_error("Buffer underflow");
            }

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
        boost::asio::streambuf _buffer;
    };
}

#endif  // TCP_READER_H
