#ifndef SESSION_H
#define SESSION_H

#include <boost/asio.hpp>
#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

#include "FileWriter.h"
#include "boost/asio/read.hpp"

class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(boost::asio::ip::tcp::socket socket, size_t client_id);

    void start();

private:
    void read_file_size();
    void read_file_name_size();
    void read_file_name(size_t file_name_size);
    void read_batch_size();
    void read_batch();
    void handle_error(const boost::system::error_code& error);

    template <class TData, class THandler>
    void read_async(TData* data, size_t size_in_bytes, THandler&& handle)
    {
        std::shared_ptr<Session> self = shared_from_this();
        boost::asio::async_read(
            _socket,
            boost::asio::buffer(data, size_in_bytes),
            [self, handle = std::forward<THandler>(handle)](const boost::system::error_code& error, size_t read_bytes)
            {
                if (error)
                {
                    self->handle_error(error);
                }
                else
                {
                    handle();
                }
            });
    }

    size_t _client_id;
    boost::asio::ip::tcp::socket _socket;

    size_t _file_size;
    FileWriter _file;

    size_t _batch_size;
    std::vector<char> _batch;
};

#endif  // SESSION_H
