#ifndef CLIENT_H
#define CLIENT_H

#include <string>

#include "../common/IApplication.h"
#include "FilePathPrompt.h"
#include "FileReader.h"
#include "TcpClient.h"

class Client : public IApplication
{
public:
    Client(unsigned short port, std::string host);

    void run() override;

private:
    /**
     * @brief Writes data to the TCP client.
     *
     * This method is responsible for sending data from the client to the server.
     * It handles the necessary operations to ensure that the data is correctly
     * transmitted over the network.
     */
    void write();

    /**
     * @brief Writes the size of the file to the TCP client.
     *
     * @param size The size of the file in bytes.
     */
    void write_file_size(size_t size);

    /**
     * @brief Writes a file name to the TCP client with a prefixed size indicator.
     *
     * Buffer structure:
     * ---------------------------------------------------------
     * | name_size (size_t) | file_name (char[])              |
     * ---------------------------------------------------------
     *
     * @param name The name of the file to be written.
     *
     * @note The receiver must first read the `size_t` value to determine
     *       the length of the file name before reading the actual name.
     *
     * @warning The use of `size_t` may lead to compatibility issues between
     *          systems with different size representations (e.g., 32-bit vs. 64-bit).
     */
    void write_file_name(const std::string& name);

    /**
     * @brief Writes file data to the TCP client.
     *
     * This method reads data from a `FileReader` object and sends it to the TCP client
     * in batches. Each batch consists of a `size_t` value indicating the number of bytes
     * read, followed by the actual file data.
     *
     * Buffer structure:
     * ---------------------------------------------------------
     * | bytes_read (size_t) | file_data (char[])             |
     * ---------------------------------------------------------
     *
     * @param file A reference to a `FileReader` object used to read file data.
     */
    void write_file(FileReader& file);

    TcpClient _tcp_client;
    FilePathPrompt _file_path_prompt;
    std::string _host;
    unsigned short _port;
};

#endif  // CLIENT_H
