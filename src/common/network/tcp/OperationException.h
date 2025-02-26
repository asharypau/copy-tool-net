#ifndef OPERATION_EXCEPTION_H
#define OPERATION_EXCEPTION_H

#include <boost/asio.hpp>
#include <string>

#include "../../../utils/Logger.h"

namespace Tcp
{
    class OperationException : public std::exception
    {
    public:
        OperationException(int error_code, const std::string message)
            : _error_code(error_code),
              _message(std::move(message))
        {
            Logger::info("OperationException constructor " + std::to_string(reinterpret_cast<uintptr_t>(this)));
        }

        OperationException(const OperationException& other)
            : _error_code(other._error_code),
              _message(other._message)
        {
            Logger::info("OperationException copy constructor " + std::to_string(reinterpret_cast<uintptr_t>(this)));
        }

        OperationException& operator=(const OperationException& other)
        {
            if (this != &other)
            {
                _error_code = other._error_code;
                _message = other._message;
            }

            Logger::info("OperationException copy assignment operator " + std::to_string(reinterpret_cast<uintptr_t>(this)));

            return *this;
        }

        OperationException(OperationException&& other) noexcept
            : _error_code(other._error_code),
              _message(std::move(other._message))
        {
            Logger::info("OperationException move constructor " + std::to_string(reinterpret_cast<uintptr_t>(this)));
        }

        OperationException& operator=(OperationException&& other) noexcept
        {
            if (this != &other)
            {
                _error_code = other._error_code;
                _message = std::move(other._message);
            }

            Logger::info("OperationException move assignment operator " + std::to_string(reinterpret_cast<uintptr_t>(this)));

            return *this;
        }

        ~OperationException() override
        {
            Logger::info("OperationException destructor " + std::to_string(reinterpret_cast<uintptr_t>(this)));
        };

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
