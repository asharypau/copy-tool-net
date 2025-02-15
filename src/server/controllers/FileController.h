#ifndef FILE_CONTROLLER_H
#define FILE_CONTROLLER_H

#include "../Session.h"
#include "services/FileService.h"

namespace Server
{
    class FileController : public IController
    {
    public:
        FileController(std::string& client_id, Tcp::Reader& tcp_reader, Tcp::Writer& tcp_writer);
        void handle(size_t request_size, std::shared_ptr<Session> session) override;

    private:
        void read_headers(std::shared_ptr<Session> session);
        void read_file(Tcp::header_type confirmation_id, std::shared_ptr<Session> session);
        void write_confirmation(Tcp::header_type confirmation_id, std::shared_ptr<Session> session);

        std::string& _client_id;
        Tcp::Reader& _tcp_reader;
        Tcp::Writer& _tcp_writer;
        FileService _file_service;
        size_t _request_size;
    };
}

#endif  // FILE_CONTROLLER_H
