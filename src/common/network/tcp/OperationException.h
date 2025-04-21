#ifndef OPERATION_EXCEPTION_H
#define OPERATION_EXCEPTION_H

#include <boost/asio.hpp>
#include <stdexcept>
#include <string>
#include <utility>

namespace Tcp
{
    class OperationException : public std::runtime_error
    {
    public:
        explicit OperationException(boost::system::error_code error_code)
            : std::runtime_error(std::move(error_code.message())),
              _error_code(error_code.value())
        {
        }

        OperationException(int error_code, const std::string message)
            : std::runtime_error(std::move(message)),
              _error_code(error_code)
        {
        }

        int error_code() const noexcept
        {
            return _error_code;
        }

    private:
        int _error_code;
    };
}

#endif  // OPERATION_EXCEPTION_H
