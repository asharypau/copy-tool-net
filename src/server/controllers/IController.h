#ifndef ICONTROLLER_H
#define ICONTROLLER_H

#include <memory>

#include "../common/network/tcp/Constants.h"

namespace Server
{
    class Session;  // forward declaration

    class IController
    {
    public:
        virtual void handle(Tcp::header_t request_size, std::shared_ptr<Session> session) = 0;

        virtual ~IController() = default;
    };

}
#endif  // ICONTROLLER_H
