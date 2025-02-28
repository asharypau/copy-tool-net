#ifndef STORAGE_PROVIDER_H
#define STORAGE_PROVIDER_H

#include <cstddef>
#include <string>

namespace Server
{
    class StorageProvider
    {
    public:
        explicit StorageProvider(const size_t client_id);

        /**
         * @brief Creates a client storage.
         */
        void create();

        const std::string& get_path() const noexcept
        {
            return _absolute_path;
        }

    private:
        static constexpr std::string_view CLIENT_STORAGE_PATH = "D:/Education/CppMentoring/files/output";

        std::string _absolute_path;
        const size_t _client_id;
    };
}

#endif  // STORAGE_PROVIDER_H
