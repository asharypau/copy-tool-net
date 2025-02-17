#ifndef TCP_CONSTANTS_H
#define TCP_CONSTANTS_H

namespace Tcp
{
    using header_t = size_t;

    static constexpr size_t HEADER_SIZE = sizeof(header_t);
    static constexpr size_t CONTENT_SIZE = sizeof(size_t);
}

#endif  // TCP_CONSTANTS_H
