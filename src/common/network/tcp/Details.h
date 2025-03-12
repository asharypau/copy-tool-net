#ifndef TCP_DETAILS_H
#define TCP_DETAILS_H

#include <boost/asio.hpp>

#include "ISerializable.h"

namespace Tcp
{
    namespace Details
    {
        template <class TModel>
        concept serializable_constraint = std::is_base_of_v<ISerializable, TModel>;
    }
}

#endif  // TCP_DETAILS_H