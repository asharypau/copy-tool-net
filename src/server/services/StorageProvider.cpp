#include "StorageProvider.h"

#include <filesystem>
#include <format>

#include "../utils/Logger.h"

using namespace Server;

StorageProvider::StorageProvider(const size_t client_id)
    : _absolute_path(),
      _client_id(client_id)
{
}

void StorageProvider::create()
{
    _absolute_path = std::format("{}/{}/", std::string(CLIENT_STORAGE_PATH), std::to_string(_client_id));
    if (std::filesystem::exists(_absolute_path))
    {
        Logger::info(std::format("Client {} storage already exists", _client_id));
    }
    else
    {
        std::filesystem::create_directory(_absolute_path);
        Logger::info(std::format("Client {} storage created", _client_id));
    }
}
