#ifndef TCP_H
#define TCP_H

#include <boost/asio.hpp>
#include <memory>
#include <string>
#include <vector>

#include "../utils/Logger.h"

namespace Tcp
{
    class ISerializable;  // forward declaration

    using header_type = size_t;

    static constexpr size_t CONTENT_SIZE = sizeof(size_t);
    static constexpr size_t HEADER_SIZE = sizeof(size_t);

    template <class TModel>
    concept Is_ISerializable = std::is_base_of_v<ISerializable, TModel>;

    namespace InternalDetail
    {
        void handle_error(const boost::system::error_code& error);
    }

    class Connector
    {
    public:
        /**
         * @brief Establishes an asynchronous TCP connection to a specified host and port.
         *
         * This template function initiates an asynchronous connection to a TCP endpoint using Boost.Asio.
         * It resolves the provided host and port, and then attempts to connect to the resolved endpoint.
         * Upon a successful connection or an error, the provided callback is invoked. If the connection
         * is successful, the callback is executed. If an error occurs, it is handled and logged.
         *
         * The function operates as follows:
         * - Resolves the given host and port to obtain a list of endpoints.
         * - Attempts to asynchronously connect to the resolved endpoint using `boost::asio::async_connect`.
         * - If the connection succeeds, it calls the provided callback.
         * - If the connection fails, the error is passed to an error handler.
         * - Catches any exceptions thrown during the process and logs them.
         *
         * @tparam TCallback The type of the callback function or callable object that is executed upon successful connection.
         *
         * @param port The port number to which the connection should be made.
         * @param host The host (IP address or domain) of the TCP server to connect to.
         * @param socket The `boost::asio::ip::tcp::socket` object used for the connection.
         * @param callback The callback function or callable object to be invoked once the connection is established.
         */
        template <class TCallback>
        static void connect(unsigned short port, std::string host, boost::asio::ip::tcp::socket& socket, TCallback&& callback)
        {
            boost::asio::ip::tcp::resolver resolver(socket.get_executor());
            boost::asio::ip::tcp::resolver::results_type endpoint = resolver.resolve(host, std::to_string(port));
            if (endpoint.empty())
            {
                Logger::error("Failed to resolve endpoint by provided " + host + ":" + std::to_string(port));

                return;
            }

            boost::asio::async_connect(
                socket,
                endpoint,
                [host, port, callback = std::forward<TCallback>(callback)](const boost::system::error_code& error, boost::asio::ip::tcp::endpoint)
                {
                    try
                    {
                        if (error)
                        {
                            InternalDetail::handle_error(error);
                        }
                        else
                        {
                            Logger::info("Connected by " + host + ":" + std::to_string(port));

                            callback();
                        }
                    }
                    catch (const std::exception& ex)
                    {
                        Logger::error("Error occurred during connect: " + std::string(ex.what()));
                    }
                });
        }
    };

    class Acceptor
    {
    public:
        Acceptor(unsigned short port, boost::asio::io_context& context);

        /**
         * @brief Initiates an asynchronous TCP connection acceptance.
         *
         * The function operates as follows:
         * - Calls `async_accept` on the `_acceptor` object to wait for an incoming client connection.
         * - If the connection attempt is successful, the provided callback is invoked with the accepted socket, allowing the caller to handle the communication.
         * - If an error occurs, the error is passed to an error handler.
         * - Catches any exceptions thrown during the process and logs them..
         *
         * @tparam TCallback The type of the callback function or callable object that is executed when a connection is successfully accepted.
         *
         * @param callback The callback function or callable object to be invoked once a connection is accepted.
         */
        template <class TCallback>
        void accept(TCallback&& callback)
        {
            _acceptor.async_accept(
                [this, callback = std::forward<TCallback>(callback)](const boost::system::error_code& error, boost::asio::ip::tcp::socket socket)
                {
                    try
                    {
                        if (error)
                        {
                            InternalDetail::handle_error(error);
                        }
                        else
                        {
                            callback(std::move(socket));
                        }
                    }
                    catch (const std::exception& ex)
                    {
                        Logger::error("Error occurred during accept: " + std::string(ex.what()));
                    }
                });
        }

    private:
        boost::asio::ip::tcp::acceptor _acceptor;
    };

    class Writer
    {
    public:
        explicit Writer(boost::asio::ip::tcp::socket& socket);

        template <class TModel, class TCallback>
        void write_async(TModel& model, TCallback&& callback)
        {
            Tcp::header_type content_length = sizeof(model);

            std::shared_ptr<std::vector<char>> buffer = std::make_shared<std::vector<char>>(Tcp::HEADER_SIZE + content_length);
            std::memcpy(buffer->data(), &content_length, Tcp::HEADER_SIZE);
            std::memcpy(buffer->data() + Tcp::HEADER_SIZE, &model, content_length);

            boost::asio::async_write(
                _socket,
                boost::asio::buffer(buffer->data(), buffer->size()),
                [this, buffer, callback = std::forward<TCallback>(callback)](const boost::system::error_code& error, size_t read_bytes) mutable
                {
                    try
                    {
                        if (error)
                        {
                            InternalDetail::handle_error(error);
                        }
                        else
                        {
                            callback();
                        }
                    }
                    catch (const std::exception& ex)
                    {
                        Logger::error("Error occurred during write: " + std::string(ex.what()));
                    }
                });
        }

        template <Is_ISerializable TModel, class TCallback>
        void write_async(TModel& model, TCallback&& callback)
        {
            std::vector<char> content = model.serialize();
            Tcp::header_type content_length = content.size();

            std::shared_ptr<std::vector<char>> buffer = std::make_shared<std::vector<char>>(Tcp::HEADER_SIZE + content_length);
            std::memcpy(buffer->data(), &content_length, Tcp::HEADER_SIZE);
            std::memcpy(buffer->data() + Tcp::HEADER_SIZE, content.data(), content_length);

            boost::asio::async_write(
                _socket,
                boost::asio::buffer(buffer->data(), buffer->size()),
                [this, buffer, callback = std::forward<TCallback>(callback)](const boost::system::error_code& error, size_t read_bytes) mutable
                {
                    try
                    {
                        if (error)
                        {
                            InternalDetail::handle_error(error);
                        }
                        else
                        {
                            callback();
                        }
                    }
                    catch (const std::exception& ex)
                    {
                        Logger::error("Error occurred during write: " + std::string(ex.what()));
                    }
                });
        }

    private:
        boost::asio::ip::tcp::socket& _socket;
    };

    class Reader
    {
    public:
        explicit Reader(boost::asio::ip::tcp::socket& socket);

        // Disallow copying and assignment.
        Reader(const Reader&) = delete;
        Reader& operator=(const Reader&) = delete;

        // Disallow movement and assignment.
        Reader(Reader&& other) = delete;
        Reader& operator=(Reader&&) = delete;

        template <class TModel, class TCallback>
        void read_async(TCallback&& callback)
        {
            boost::asio::async_read(
                _socket,
                _buffer,
                boost::asio::transfer_exactly(HEADER_SIZE),
                [this, callback = std::forward<TCallback>(callback)](const boost::system::error_code& error, size_t read_bytes)
                {
                    try
                    {
                        if (error)
                        {
                            InternalDetail::handle_error(error);
                        }
                        else
                        {
                            Tcp::header_type content_length = 0;
                            extract(&content_length, CONTENT_SIZE);

                            read(content_length);
                            TModel model;
                            extract(&model, sizeof(TModel));

                            callback(model);
                        }
                    }
                    catch (const std::exception& ex)
                    {
                        Logger::error("Error occurred during read: " + std::string(ex.what()));
                    }
                });
        }

        template <Is_ISerializable TModel, class TCallback>
        void read_async(TCallback&& callback)
        {
            boost::asio::async_read(
                _socket,
                _buffer,
                boost::asio::transfer_exactly(HEADER_SIZE),
                [this, callback = std::forward<TCallback>(callback)](const boost::system::error_code& error, size_t read_bytes)
                {
                    try
                    {
                        if (error)
                        {
                            InternalDetail::handle_error(error);
                        }
                        else
                        {
                            Tcp::header_type content_length = 0;
                            extract(&content_length, CONTENT_SIZE);

                            read(content_length);
                            TModel model = extract<TModel>(content_length);

                            callback(model);
                        }
                    }
                    catch (const std::exception& ex)
                    {
                        Logger::error("Error occurred during read: " + std::string(ex.what()));
                    }
                });
        }

    private:
        /**
         * @brief Performs a synchronous read operation on the TCP socket.
         *
         * The function operates as follows:
         * - Calls `boost::asio::read` to read exactly `size_in_bytes` bytes from the socket.
         * - If the read operation succeeds, the data is stored in `_buffer`.
         * - If an error occurs, the error is passed to an error handler.
         *
         * @param size_in_bytes The exact number of bytes to read from the socket.
         */
        void read(size_t size_in_bytes);

        /**
         * @brief Extracts a specified number of bytes from the internal buffer.
         *
         * The function operates as follows:
         * - Uses `boost::asio::buffer_cast` to obtain a raw pointer to the buffer data.
         * - Consumes (removes) the extracted bytes from `_buffer`.
         *
         * @tparam TData The type of data being extracted.
         * @param data A pointer to the destination memory where the extracted bytes will be stored.
         * @param size_in_bytes The number of bytes to extract from `_buffer`.
         */
        template <class TData>
        void extract(TData* data, size_t size_in_bytes)
        {
            if (_buffer.size() < size_in_bytes)
            {
                throw std::runtime_error("Buffer underflow");
            }

            const TData* raw_data = boost::asio::buffer_cast<const TData*>(_buffer.data());
            std::memcpy(data, raw_data, size_in_bytes);

            _buffer.consume(size_in_bytes);
        }

        template <Is_ISerializable TModel>
        TModel extract(Tcp::header_type content_length)
        {
            TModel model(content_length);
            Tcp::header_type consumed_bytes = model.deserialize(_buffer);
            if (consumed_bytes != content_length)
            {
                throw std::runtime_error("Buffer underflow");
            }

            _buffer.consume(consumed_bytes);

            return model;
        }

        boost::asio::ip::tcp::socket& _socket;
        boost::asio::streambuf _buffer;
    };

    class ISerializable
    {
    public:
        ISerializable() = default;
        ISerializable(Tcp::header_type content_length)
            : _content_length(content_length)
        {
        }

        virtual ~ISerializable() = default;

        virtual std::vector<char> serialize() = 0;
        virtual Tcp::header_type deserialize(const boost::asio::streambuf& buffer) = 0;

        Tcp::header_type get_content_length()
        {
            return _content_length;
        }

    private:
        Tcp::header_type _content_length;
    };
}

#endif  // TCP_H
