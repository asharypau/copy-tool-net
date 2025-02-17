#ifndef TCP_UTILS_H
#define TCP_UTILS_H

#include <boost/asio.hpp>

#include "../../../utils/Logger.h"
#include "ISerializable.h"

namespace Tcp
{
    namespace Utils
    {
        template <class TModel>
        concept serializable_constraint = std::is_base_of_v<ISerializable, TModel>;

        inline void handle_error(const boost::system::error_code& error)
        {
            if (error == boost::asio::error::eof)
            {
                Logger::info("Disconnected.");
            }
            else if (error == boost::asio::error::connection_reset)
            {
                Logger::error("The connection was reset by the remote side.");
            }
            else
            {
                Logger::error("An unknown error occurred.");
            }
        }
    }
}

#endif  // TCP_UTILS_H