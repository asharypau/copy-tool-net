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
        virtual ~ISerializable() = default;

        virtual std::vector<std::byte> serialize() = 0;
        virtual Tcp::header_t deserialize(boost::beast::flat_buffer& buffer) = 0;
    };
}

#endif  // TCP_ISERIALIZEBLE_H
