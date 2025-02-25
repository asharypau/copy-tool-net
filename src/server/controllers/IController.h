#ifndef ICONTROLLER_H
#define ICONTROLLER_H

#include <boost/asio.hpp>

#include "../common/network/tcp/Constants.h"

namespace Server
{
    class IController
    {
    public:
        /**
         * @brief Handles an incoming request by initializing the read operation.
         *
         * @param request_size The size of the incoming request in bytes.
         * @param session A shared pointer to the session handling the request.
         */
        virtual boost::asio::awaitable<void> handle(Tcp::header_t request_size) = 0;

        virtual ~IController() = default;
    };

}
#endif  // ICONTROLLER_H
