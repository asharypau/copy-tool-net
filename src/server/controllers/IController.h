#ifndef ICONTROLLER_H
#define ICONTROLLER_H

#include <memory>

namespace Server
{
    class Session;  // forward declaration

    class IController
    {
    public:
        virtual void handle(size_t request_size, std::shared_ptr<Session> session) = 0;

        virtual ~IController() = default;
    };

}
#endif  // ICONTROLLER_H
