#ifndef FILE_SERVICE_H
#define FILE_SERVICE_H

#include <fstream>
#include <string>

class FileService
{
public:
    FileService();

    /**
     * @brief Opens or creates a file at the specified path.
     *
     * This function attempts to open an existing file at the given path. If the file does not exist,
     * it will be created. The function ensures that the file is ready for use after the operation.
     *
     * @param path The path to the file to be opened or created.
     *
     * @throws std::runtime_error If an error occurs while opening or creating the file.
     */
    void open_create(const std::string& path);

    /**
     * @brief Opens a file for reading at the specified path.
     *
     * This function attempts to open an existing file in read mode.
     *
     * @param path The path to the file to be opened for reading.
     *
     * @throws std::runtime_error If an error occurs while opening the file for reading
     * (e.g., file does not exist and so on).
     */
    void open_read(const std::string& path);

    /**
     * @brief Closes the file.
     *
     * @throws std::runtime_error If an error occurs while closing the file.
     */
    void close();

    /**
     * @brief Writes data to a file.
     *
     * @param data Pointer to the data to be written.
     * @param size The size of the data to be written, in bytes.
     *
     * @throws std::runtime_error If an error occurs while writing the file.
     * (e.g., insufficient space, write failure).
     */
    void write(const char* data, std::size_t size);

    /**
     * @brief Reads data from a file.
     *
     * @param data Pointer to the buffer where the read data will be stored.
     * @param size The maximum number of bytes to read.
     *
     * @return std::size_t The number of bytes actually read.
     *
     * @throws std::runtime_error If an error occurs while reading the file.
     * (e.g., read failure).
     */
    std::size_t read(char* date, std::size_t size);

private:
    /**
     * @brief Gets an available file version.
     *
     * @param path The path to the file or resource from which to retrieve the version.
     *
     * @return std::string A string containing the version.
     */
    std::string get_version(const std::string& path);

    std::fstream _file;
};

#endif  // FILE_SERVICE_H