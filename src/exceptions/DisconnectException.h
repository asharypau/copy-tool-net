#ifndef DISCONNECT_EXCEPTION_H
#define DISCONNECT_EXCEPTION_H

#include <exception>
#include <string>

class DisconnectException : public std::exception
{
public:
    DisconnectException(const std::string& message) : _message(message) {}

    const char* what() const noexcept override
    {
        return _message.c_str();
    }

private:
    const std::string _message;
};

#endif  // DISCONNECT_EXCEPTION_H
