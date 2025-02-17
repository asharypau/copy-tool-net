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
        template <class TModel, class TCallback>
        void write_async(TModel& model, TCallback&& callback)
        {
            Tcp::header_t content_length = sizeof(model);

            internal_write_async(content_length, &model, std::forward<TCallback>(callback));
        }

        template <Tcp::Utils::serializable_constraint TSerializableModel, class TCallback>
        void write_async(TSerializableModel& model, TCallback&& callback)
        {
            std::vector<std::byte> content = model.serialize();
            Tcp::header_t content_length = content.size();

            internal_write_async(content_length, content.data(), std::forward<TCallback>(callback));
        }

    private:
        boost::asio::ip::tcp::socket& _socket;

        template <class TContent>
        std::shared_ptr<std::vector<std::byte>> get_buffer(Tcp::header_t content_length, const TContent* raw_content) const
        {
            std::vector<std::byte> buffer = std::vector<std::byte>(Tcp::HEADER_SIZE + content_length);
            std::memcpy(buffer.data(), &content_length, Tcp::HEADER_SIZE);
            std::memcpy(buffer.data() + Tcp::HEADER_SIZE, raw_content, content_length);

            return std::make_shared<std::vector<std::byte>>(std::move(buffer));
        }

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
