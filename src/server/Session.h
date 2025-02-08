#ifndef SESSION_H
#define SESSION_H

#include <boost/asio.hpp>
#include <memory>

#include "../utils/Logger.h"
#include "FileHandler.h"

class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(boost::asio::ip::tcp::socket socket, size_t client_id);

    void run();

private:
    /**
     * @brief Asynchronously reads the file size and initializes a file handler.
     *
     * This method begins an asynchronous read operation to retrieve the file size.
     * Once the size is read, a `FileHandler` instance is created using the obtained
     * size. The method then proceeds to read the file name.
     *
     * The workflow is as follows:
     * 1. Initiates an asynchronous read for the file size.
     * 2. Calls `read_size_from_buffer()` to extract the size from the buffer.
     * 3. Creates a `FileHandler` instance using the retrieved file size.
     * 4. Calls `read_file_name()` to continue the file reception process.
     */
    void read_file_size();

    /**
     * @brief Asynchronously reads the file name and initializes the file.
     *
     * This method first reads the size of the file name asynchronously, then reads
     * the actual file name data. Once the file name is received, a `FileHandler`
     * instance is used to create the file. The method then proceeds to read the file
     * content in batches.
     *
     * The workflow is as follows:
     * 1. Reads the file name size asynchronously.
     * 2. Extracts the size from the buffer using `read_size_from_buffer()`.
     * 3. Reads the file name asynchronously based on the extracted size.
     * 4. Converts the received data into a `std::string` and passes it to `FileHandler::create()`.
     * 5. Calls `read_batch()` to continue reading the file content.
     *
     * @param file A shared pointer to a `FileHandler` instance that will manage the file.
     */
    void read_file_name(std::shared_ptr<FileHandler> file);

    /**
     * @brief Asynchronously reads a batch of file data and writes it to the file.
     *
     * This method reads a chunk of file data in an asynchronous manner. It first
     * retrieves the size of the incoming data, then reads the actual data and writes
     * it to the provided `FileHandler` instance. If there is more data to receive,
     * the method recursively calls itself to continue reading. Otherwise, it restarts
     * the file reception process by calling `read_file_size()`.
     *
     * The workflow is as follows:
     * 1. Reads the next chunk size asynchronously.
     * 2. Extracts the size from the buffer using `read_size_from_buffer()`.
     * 3. Reads the chunk of file data asynchronously.
     * 4. Writes the received data to the `FileHandler` instance.
     * 5. If more data remains, calls `read_batch()` to continue reading.
     * 6. If the file transfer is complete, calls `read_file_size()` to prepare for a new file.
     *
     * @param file A shared pointer to a `FileHandler` instance that manages writing to the file.
     */
    void read_batch(std::shared_ptr<FileHandler> file);

    size_t read_size_from_buffer();
    void handle_error(const boost::system::error_code& error);

    template <class TCallback>
    void get_size(TCallback&& callback)
    {
        get_data(sizeof(size_t), std::forward<TCallback>(callback));
    }

    /**
     * @brief Asynchronously reads a specified amount of data from the socket.
     *
     * This method initiates an asynchronous read operation using Boost.Asio. It
     * reads exactly `size_in_bytes` from the socket into `_buffer` and then invokes
     * the provided handler upon completion.
     *
     * The method captures `self` via `shared_from_this()` to ensure the session remains
     * valid during the asynchronous operation. If an error occurs during reading,
     * it is handled by `handle_error()`. Otherwise, the provided handler is executed.
     *
     * @tparam THandler The type of the handler function to be called after reading.
     *
     * @param size_in_bytes The exact number of bytes to read from the socket.
     * @param handle A callable object (e.g., lambda function) that is executed upon successful reading.
     */
    template <class TCallback>
    void get_data(size_t size_in_bytes, TCallback&& callback)
    {
        std::shared_ptr<Session> self = shared_from_this();
        boost::asio::async_read(
            _socket,
            _buffer,
            boost::asio::transfer_exactly(size_in_bytes),
            [self, callback = std::forward<TCallback>(callback)](const boost::system::error_code& error, size_t read_bytes)
            {
                try
                {
                    if (error)
                    {
                        self->handle_error(error);
                    }
                    else
                    {
                        callback();
                    }
                }
                catch (const std::exception& ex)
                {
                    Logger::error("Error occurred: " + std::string(ex.what()));
                }
            });
    }

    size_t _client_id;
    boost::asio::ip::tcp::socket _socket;
    boost::asio::streambuf _buffer;
};

#endif  // SESSION_H
