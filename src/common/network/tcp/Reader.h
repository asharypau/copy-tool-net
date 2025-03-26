#ifndef TCP_READER_H
#define TCP_READER_H

#include <boost/asio.hpp>
#include <cstddef>
#include <tuple>
#include <utility>
#include <vector>

#include "ISerializable.h"
#include "Socket.h"

namespace Tcp
{
    class Reader
    {
    public:
        explicit Reader(Tcp::Socket& socket)
            : _socket(socket)
        {
        }

        // Disallow copying and assignment.
        Reader(const Reader&) = delete;
        Reader& operator=(const Reader&) = delete;

        // Disallow movement and assignment.
        Reader(Reader&&) = delete;
        Reader& operator=(Reader&&) = delete;

        template <class TModel>
        boost::asio::awaitable<std::tuple<TModel, size_t>> read_async()
        {
            std::vector<unsigned char> bytes = co_await _socket.read_async();
            TModel model;

            if constexpr (std::is_base_of_v<Tcp::ISerializable, TModel>)
            {
                model.deserialize(bytes);
            }
            else
            {
                std::memcpy(&model, bytes.data(), bytes.size());
            }

            std::tuple<TModel, size_t> response = std::make_tuple(std::move(model), bytes.size());

            co_return std::move(response);
        }

    private:
        Tcp::Socket& _socket;
    };
}

#endif  // TCP_READER_H
