#ifndef TCP_H
#define TCP_H

#include <boost/asio.hpp>
#include <string>

#include "../utils/Logger.h"

namespace Tcp
{
    static constexpr size_t HEADER_SIZE = sizeof(size_t);

    namespace InternalDetail
    {
        void handle_error(const boost::system::error_code& error);
    }

    class Connector
    {
    public:
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

        template <class TData, class TCallback>
        void write(TData* data, size_t size_in_bytes, TCallback&& callback)
        {
            boost::asio::async_write(
                _socket,
                boost::asio::buffer(data, size_in_bytes),
                [this, callback = std::forward<TCallback>(callback)](const boost::system::error_code& error, size_t read_bytes) mutable
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

        void read(size_t size_in_bytes);

        template <class TCallback>
        void read_async(size_t size_in_bytes, TCallback&& callback)
        {
            boost::asio::async_read(
                _socket,
                _buffer,
                boost::asio::transfer_exactly(size_in_bytes),
                [this, callback = std::forward<TCallback>(callback)](const boost::system::error_code& error, size_t read_bytes) mutable
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
                        Logger::error("Error occurred during read: " + std::string(ex.what()));
                    }
                });
        }

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

    private:
        boost::asio::ip::tcp::socket& _socket;
        boost::asio::streambuf _buffer;
    };
}

#endif  // TCP_H
