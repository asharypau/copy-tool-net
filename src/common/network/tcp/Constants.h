#ifndef TCP_CONSTANTS_H
#define TCP_CONSTANTS_H

namespace Tcp
{
    using header_t = size_t;

    static constexpr size_t HEADER_SIZE = sizeof(header_t);

    enum HandshakeType
    {
        UNKNOWN = 0,
        SERVER = 1,
        CLIENT = 2
    };
}

#endif  // TCP_CONSTANTS_H
