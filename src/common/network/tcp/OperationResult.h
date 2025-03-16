#ifndef OPERATION_RESULT_H
#define OPERATION_RESULT_H

#include <optional>
#include <string>
#include <utility>

#include "boost/system/detail/error_code.hpp"

namespace Tcp
{
    template <class TResult>
    class OperationResult
    {
    public:
        explicit OperationResult(TResult res)
            : _result(std::move(res)),
              _error_code(0),
              _error_message()
        {
        }

        explicit OperationResult(boost::system::error_code& err)
            : _result(),
              _error_code(err.value()),
              _error_message(err.message())
        {
            if (_error_message == "The operation completed successfully")
            {
                _error_message.clear();
            }
        }

        OperationResult(TResult res, boost::system::error_code& err)
            : _result(std::move(res)),
              _error_code(err.value()),
              _error_message(err.message())
        {
            if (_error_message == "The operation completed successfully")
            {
                _error_message.clear();
            }
        }

        OperationResult(TResult res, int err_code, std::string err_msg)
            : _result(std::move(res)),
              _error_code(err_code),
              _error_message(err_msg)
        {
        }

        OperationResult(int err_code, std::string err_msg)
            : _result(),
              _error_code(err_code),
              _error_message(err_msg)
        {
        }

        explicit operator bool() const noexcept
        {
            return _result.has_value() && _error_code == 0 && _error_message.empty();
        }

        const TResult& result() const
        {
            return _result.value();
        }

        TResult acquire_result()
        {
            TResult temp = std::move(_result.value());
            _result.reset();

            return std::move(temp);
        }

        int error_code() const noexcept
        {
            return _error_code;
        }

        const std::string& error_message() const noexcept
        {
            return _error_message;
        }

    private:
        std::optional<TResult> _result;
        int _error_code;
        std::string _error_message;
    };
}

#endif  // OPERATION_RESULT_H
