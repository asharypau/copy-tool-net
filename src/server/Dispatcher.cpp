#include "Dispatcher.h"

#include "../common/network/Endpoints.h"
#include "controllers/FileController.h"

using namespace Server;

Dispatcher::Dispatcher(Tcp::Reader& tcp_reader, Tcp::Writer& tcp_writer, const StorageProvider& storage_provider)
    : _controllers()
{
    _controllers.emplace(std::string(Endpoints::FILE), std::make_unique<FileController>(tcp_reader, tcp_writer, storage_provider));
}

boost::asio::awaitable<void> Dispatcher::handle(RequestMetadata& request_metadata)
{
    const std::unordered_map<std::string, std::unique_ptr<IController>>::iterator& it = _controllers.find(request_metadata.endpoint);
    if (it != _controllers.end())
    {
        co_await it->second->handle(request_metadata.size);
    }
    else
    {
        Logger::error("Invalid endpoint: " + request_metadata.endpoint);
    }
}
