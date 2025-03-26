#ifndef TCP_ISERIALIZEBLE_H
#define TCP_ISERIALIZEBLE_H

#include <vector>

namespace Tcp
{
    class ISerializable
    {
    public:
        virtual ~ISerializable() = default;

        virtual std::vector<unsigned char> serialize() = 0;
        virtual void deserialize(const std::vector<unsigned char>& bytes) = 0;
    };
}

#endif  // TCP_ISERIALIZEBLE_H
