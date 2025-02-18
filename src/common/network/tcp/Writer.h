#ifndef TCP_WRITER_H
#define TCP_WRITER_H

#include <boost/asio.hpp>
#include <string>

#include "../../../utils/Logger.h"
#include "Constants.h"
#include "Utils.h"

namespace Tcp
{
    class Writer
    {
    public:
        explicit Writer(boost::asio::ip::tcp::socket& socket)
            : _socket(socket)
        {
        }

        /**
         * @brief Performs an asynchronous write operation to the TCP socket.
         *
         * The function operates as follows:
         * - Determines the size of the model in bytes.
         * - Calls `internal_write_async` to send the model data along with its size.
         *
         * @tparam TModel The type of the model to be written.
         * @tparam TCallback The type of the callback function to be invoked upon completion.
         * @param model The model instance to be written to the socket.
         * @param callback The callback function to be executed once the operation completes.
         */
        template <class TModel, class TCallback>
        void write_async(TModel& model, TCallback&& callback)
        {
            Tcp::header_t content_length = sizeof(model);

            internal_write_async(content_length, &model, std::forward<TCallback>(callback));
        }

        /**
         * @brief Performs an asynchronous write operation to the TCP socket.
         *
         * The function operates as follows:
         * - Serializes the provided model into a byte vector.
         * - Determines the size of the serialized data.
         * - Calls `internal_write_async` to send the serialized data along with its size.
         *
         * @tparam TSerializableModel The type of the model to be extracted from the received data, constrained to be serializable.
         * @tparam TCallback The type of the callback function to be invoked upon completion.
         * @param model The serializable model instance to be written to the socket.
         * @param callback The callback function to be executed once the operation completes.
         */
        template <Tcp::Utils::serializable_constraint TSerializableModel, class TCallback>
        void write_async(TSerializableModel& model, TCallback&& callback)
        {
            std::vector<std::byte> content = model.serialize();
            Tcp::header_t content_length = content.size();

            internal_write_async(content_length, content.data(), std::forward<TCallback>(callback));
        }

    private:
        boost::asio::ip::tcp::socket& _socket;

        /**
         * @brief Creates a shared buffer containing serialized content prefixed with its size.
         *
         * The function operates as follows:
         * - Allocates a buffer with enough space to store a header (`Tcp::HEADER_SIZE`) and the content.
         * - Copies the content length into the header section of the buffer.
         * - Copies the raw content into the remaining portion of the buffer.
         * - Returns a shared pointer to the allocated buffer.
         *
         * @tparam TContent The type of the content to be stored in the buffer.
         * @param content_length The size of the content in bytes.
         * @param raw_content A pointer to the raw content to be stored in the buffer.
         * @return A shared pointer to a `std::vector<std::byte>` containing the formatted buffer.
         */
        template <class TContent>
        std::shared_ptr<std::vector<std::byte>> get_buffer(Tcp::header_t content_length, const TContent* raw_content) const
        {
            std::vector<std::byte> buffer = std::vector<std::byte>(Tcp::HEADER_SIZE + content_length);
            std::memcpy(buffer.data(), &content_length, Tcp::HEADER_SIZE);
            std::memcpy(buffer.data() + Tcp::HEADER_SIZE, raw_content, content_length);

            return std::make_shared<std::vector<std::byte>>(std::move(buffer));
        }

        /**
         * @brief Performs an asynchronous write operation to the TCP socket.
         *
         * The function operates as follows:
         * - Constructs a buffer containing the content length header and the raw content.
         * - Initiates an asynchronous write operation using `boost::asio::async_write`.
         * - If the write operation succeeds, the provided callback function is invoked.
         * - If an error occurs during the write operation, it is handled appropriately.
         *
         * @tparam TContent The type of the content to be written.
         * @tparam TCallback The type of the callback function to be invoked upon completion.
         * @param content_length The size of the content in bytes.
         * @param raw_content A pointer to the raw content to be written.
         * @param callback The callback function to be executed once the operation completes.
         */
        template <class TContent, class TCallback>
        void internal_write_async(Tcp::header_t content_length, const TContent* raw_content, TCallback&& callback)
        {
            std::shared_ptr<std::vector<std::byte>> buffer = get_buffer(content_length, raw_content);
            boost::asio::async_write(
                _socket,
                boost::asio::buffer(*buffer),
                [this, buffer, callback = std::forward<TCallback>(callback)](const boost::system::error_code& error, size_t)
                {
                    try
                    {
                        if (error)
                        {
                            Tcp::Utils::handle_error(error);
                        }
                        else
                        {
                            callback();
                        }
                    }
                    catch (const std::exception& ex)
                    {
                        Logger::error("Error occurred during write: " + std::string(ex.what()));
                    }
                });
        }
    };
}

#endif  // TCP_WRITER_H
