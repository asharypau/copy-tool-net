#ifndef TCP_WRITER_H
#define TCP_WRITER_H

#include <boost/asio.hpp>

#include "Constants.h"
#include "Details.h"
#include "OperationException.h"

namespace Tcp
{
    class Writer
    {
    public:
        explicit Writer(boost::asio::ip::tcp::socket& socket)
            : _socket(socket)
        {
        }

        template <class TModel>
        boost::asio::awaitable<void> write_async(TModel& model)
        {
            Tcp::header_t content_length = sizeof(model);

            co_await internal_write_async(content_length, &model);
        }

        template <Tcp::Details::serializable_constraint TSerializableModel>
        boost::asio::awaitable<void> write_async(TSerializableModel& model)
        {
            std::vector<std::byte> content = model.serialize();
            Tcp::header_t content_length = content.size();

            co_await internal_write_async(content_length, content.data());
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

        template <class TContent>
        boost::asio::awaitable<void> internal_write_async(Tcp::header_t content_length, const TContent* raw_content)
        {
            std::shared_ptr<std::vector<std::byte>> buffer = get_buffer(content_length, raw_content);
            auto [error, _] = co_await boost::asio::async_write(_socket, boost::asio::buffer(*buffer), boost::asio::as_tuple(boost::asio::use_awaitable));

            if (error)
            {
                throw Tcp::OperationException(error);
            }
        }
    };
}

#endif  // TCP_WRITER_H
