#ifndef OPERATION_EXCEPTION_H
#define OPERATION_EXCEPTION_H

#include <boost/asio.hpp>
#include <string>

namespace Tcp
{
    class OperationException : public std::exception
    {
    public:
        OperationException(int error_code, const std::string message)
            : _error_code(error_code),
              _message(std::move(message))
        {
        }

        int get_error_code() const noexcept
        {
            return _error_code;
        }

        const char* what() const noexcept override
        {
            return _message.c_str();
        }

    private:
        int _error_code;
        std::string _message;
    };
}

#endif  // OPERATION_EXCEPTION_H
