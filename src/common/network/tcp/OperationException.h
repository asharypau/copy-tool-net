#ifndef OPERATION_EXCEPTION_H
#define OPERATION_EXCEPTION_H

#include <boost/asio.hpp>
#include <stdexcept>
#include <string>

namespace Tcp
{
    class OperationException : public std::runtime_error
    {
    public:
        explicit OperationException(boost::system::error_code& err)
            : std::runtime_error(err.message()),
              _error_code(err.value())
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
