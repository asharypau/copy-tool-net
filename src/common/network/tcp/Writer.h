#ifndef TCP_WRITER_H
#define TCP_WRITER_H

#include <boost/asio.hpp>

#include "ISerializable.h"
#include "Socket.h"

namespace Tcp
{
    class Writer
    {
    public:
        explicit Writer(Tcp::Socket& socket)
            : _socket(socket)
        {
        }

        // Disallow copying and assignment.
        Writer(const Writer&) = delete;
        Writer& operator=(const Writer&) = delete;

        // Disallow movement and assignment.
        Writer(Writer&&) = delete;
        Writer& operator=(Writer&&) = delete;

        template <class TModel>
        boost::asio::awaitable<void> write_async(TModel& model)
        {
            if constexpr (std::is_base_of_v<ISerializable, TModel>)
            {
                std::vector<unsigned char> bytes = model.serialize();
                co_await _socket.write_async(bytes);
            }
            else
            {
                std::vector<unsigned char> bytes(sizeof(model));
                std::memcpy(bytes.data(), &model, sizeof(model));
                co_await _socket.write_async(bytes);
            }
        }

    private:
        Tcp::Socket& _socket;
    };
}

#endif  // TCP_WRITER_H
