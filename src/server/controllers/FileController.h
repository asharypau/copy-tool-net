#ifndef FILE_CONTROLLER_H
#define FILE_CONTROLLER_H

#include <cstddef>

#include "../Session.h"
#include "IController.h"
#include "services/FileHandler.h"

namespace Server
{
    class FileController : public IController
    {
    public:
        FileController(std::string& client_id, Tcp::Reader& tcp_reader, Tcp::Writer& tcp_writer);
        void handle(size_t request_size, std::shared_ptr<Session> session) override;

    private:
        void read_headers(std::shared_ptr<Session> session);
        void read_file(std::unique_ptr<FileHandler>&& file, std::shared_ptr<Session> session);
        void write_confirmation(std::shared_ptr<Session> session);

        std::string& _client_id;
        Tcp::Reader& _tcp_reader;
        Tcp::Writer& _tcp_writer;
        std::vector<char> _batch;
        size_t _request_size;
        size_t _confirmation_id;
    };
}

#endif  // FILE_CONTROLLER_H
