#ifndef CLIENT_MESSAGES_PROMPT_H
#define CLIENT_MESSAGES_PROMPT_H

#include <vector>

#include "../models/Message.h"
namespace Client
{

    class MessagesPrompt
    {
    public:
        std::vector<Message> get() const;

    private:
        std::vector<Message> internal_get() const;
    };
}

#endif  // CLIENT_MESSAGES_PROMPT_H
