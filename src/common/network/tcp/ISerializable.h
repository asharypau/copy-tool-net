#ifndef TCP_ISERIALIZEBLE_H
#define TCP_ISERIALIZEBLE_H

#include <vector>

namespace Tcp
{
    template <class Serializable>
    class ISerializable
    {
    public:
        std::vector<unsigned char> serialize()
        {
            return std::move(get_impl()->serialize_impl());
        }

        void deserialize(const std::vector<unsigned char>& bytes)
        {
            get_impl()->deserialize_impl(bytes);
        }

    private:
        Serializable* get_impl()
        {
            return static_cast<Serializable*>(this);
        }
    };
}

#endif  // TCP_ISERIALIZEBLE_H
