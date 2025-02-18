#ifndef FILE_CONTROLLER_H
#define FILE_CONTROLLER_H

#include "../Session.h"
#include "../common/network/tcp/Reader.h"
#include "../common/network/tcp/Writer.h"
#include "../common/services/FileService.h"

namespace Server
{
    class FileController : public IController
    {
    public:
        FileController(std::string& client_id, Tcp::Reader& tcp_reader, Tcp::Writer& tcp_writer);

        /**
         * @brief Handles an incoming request by initializing the read operation.
         *
         * The function operates as follows:
         * - Checks if the request size is nonzero.
         * - If valid, stores the request size and initiates header reading via `read_headers`.
         *
         * @param request_size The size of the incoming request in bytes.
         * @param session A shared pointer to the session handling the request.
         */
        void handle(Tcp::header_t request_size, std::shared_ptr<Session> session) override;

    private:
        /**
         * @brief Initiates an asynchronous read operation for file headers and processes them.
         *
         * The function operates as follows:
         * - Calls `read_async` to asynchronously read the file headers.
         * - Upon successful reading, it processes the headers by:
         *   - Reducing the remaining request size by the content length in the headers.
         *   - Calling `open_create` to prepare the file for writing based on the received header information.
         *   - Initiates the reading of the file data via `read_file`.
         *
         * @param session A shared pointer to the session handling the request.
         */
        void read_headers(std::shared_ptr<Session> session);

        /**
         * @brief Initiates an asynchronous read operation for file data and processes it in batches.
         *
         * The function operates as follows:
         * - Calls `read_async` to asynchronously read a file request.
         * - Upon successful reading, it processes the request by:
         *   - Reducing the remaining request size by the content length in the request.
         *   - Writing the received data batch to the file using `file_service`.
         * - If there is more data to be read, it recursively calls `read_file` to continue the operation.
         * - Once all data is read, it closes the file and sends a confirmation via `write_confirmation`.
         *
         * @param confirmation_id The ID used for confirming the file transfer.
         * @param session A shared pointer to the session handling the request.
         */
        void read_file(Tcp::header_t confirmation_id, std::shared_ptr<Session> session);

        /**
         * @brief Initiates an asynchronous write operation to confirm file transfer completion.
         *
         * The function operates as follows:
         * - Calls `write_async` to asynchronously send a confirmation ID to the client.
         * - Once the write operation completes, the provided session is notified that the process is done.
         *
         * @param confirmation_id The ID used to confirm the file transfer completion.
         * @param session A shared pointer to the session handling the request.
         */
        void write_confirmation(Tcp::header_t confirmation_id, std::shared_ptr<Session> session);

        std::string& _client_id;
        Tcp::Reader& _tcp_reader;
        Tcp::Writer& _tcp_writer;
        FileService _file_service;
        Tcp::header_t _request_size;
    };
}

#endif  // FILE_CONTROLLER_H
