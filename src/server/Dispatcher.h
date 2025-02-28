#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <memory>
#include <string>
#include <unordered_map>

#include "../common/models/RequestMetadata.h"
#include "../common/network/tcp/Reader.h"
#include "../common/network/tcp/Writer.h"
#include "controllers/IController.h"
#include "services/StorageProvider.h"

namespace Server
{
    class Dispatcher
    {
    public:
        Dispatcher(Tcp::Reader& tcp_reader, Tcp::Writer& tcp_writer, const StorageProvider& storage_provider);

        /**
         * @brief Handles an incoming request by dispatching it to the appropriate controller.
         *
         * @param request_metadata Request metadata.
         * @param session A shared pointer to the session handling the request.
         */
        boost::asio::awaitable<void> handle(RequestMetadata& request_metadata);

    private:
        std::unordered_map<std::string, std::unique_ptr<IController>> _controllers;
    };
}

#endif  // DISPATCHER_H
