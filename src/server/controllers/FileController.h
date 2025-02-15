#ifndef FILE_CONTROLLER_H
#define FILE_CONTROLLER_H

#include "../Session.h"
#include "../common/FileService.h"

namespace Server
{
    class FileController : public IController
    {
    public:
        FileController(std::string& client_id, Tcp::Reader& tcp_reader, Tcp::Writer& tcp_writer);
        void handle(Tcp::header_t request_size, std::shared_ptr<Session> session) override;

    private:
        void read_headers(std::shared_ptr<Session> session);
        void read_file(Tcp::header_t confirmation_id, std::shared_ptr<Session> session);
        void write_confirmation(Tcp::header_t confirmation_id, std::shared_ptr<Session> session);

        std::string& _client_id;
        Tcp::Reader& _tcp_reader;
        Tcp::Writer& _tcp_writer;
        FileService _file_service;
        Tcp::header_t _request_size;
    };
}

#endif  // FILE_CONTROLLER_H
