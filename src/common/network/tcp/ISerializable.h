#ifndef TCP_ISERIALIZEBLE_H
#define TCP_ISERIALIZEBLE_H

#include <boost/asio.hpp>
#include <boost/beast.hpp>

#include "Constants.h"

namespace Tcp
{
    class ISerializable
    {
    public:
        ISerializable() = default;
        ISerializable(Tcp::header_t content_length)
            : _content_length(content_length)
        {
        }

        virtual ~ISerializable() = default;

        virtual std::vector<std::byte> serialize() = 0;
        virtual Tcp::header_t deserialize(boost::beast::flat_buffer& buffer) = 0;

        Tcp::header_t get_content_length()
        {
            return _content_length;
        }

    private:
        Tcp::header_t _content_length;
    };
}

#endif  // TCP_ISERIALIZEBLE_H
