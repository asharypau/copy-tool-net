#include "Dispatcher.h"

#include "../common/network/Endpoints.h"
#include "Session.h"
#include "controllers/FileController.h"

using namespace Server;

Dispatcher::Dispatcher(std::string& client_id, Tcp::Reader& tcp_reader, Tcp::Writer& tcp_writer)
    : _controllers()
{
    _controllers.emplace(std::string(Endpoints::FILE), std::make_unique<FileController>(client_id, tcp_reader, tcp_writer));
}

void Dispatcher::handle(RequestMetadata& request_metadata, std::shared_ptr<Session> session)
{
    const std::unordered_map<std::string, std::unique_ptr<IController>>::iterator& it = _controllers.find(request_metadata.endpoint);
    if (it != _controllers.end())
    {
        it->second->handle(request_metadata.size, session);
    }
    else
    {
        Logger::error("Invalid endpoint: " + request_metadata.endpoint);
    }
}
