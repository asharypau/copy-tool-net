#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <memory>
#include <string>
#include <unordered_map>

#include "../common/Tcp.h"
#include "controllers/IController.h"

namespace Server
{
    class Session;  // forward declaration

    class Dispatcher
    {
    public:
        Dispatcher(std::string& client_id, Tcp::Reader& tcp_reader, Tcp::Writer& tcp_writer);

        /**
         * @brief Handles an incoming request by dispatching it to the appropriate controller.
         *
         * @param request_size The size of the incoming request in bytes.
         * @param endpoint The endpoint associated with the request.
         * @param session A shared pointer to the session handling the request.
         */
        void handle(size_t request_size, const std::string& endpoint, std::shared_ptr<Session> session);

    private:
        std::unordered_map<std::string, std::unique_ptr<IController>> _controllers;
    };
}

#endif  // DISPATCHER_H
