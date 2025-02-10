#ifndef CLIENT_MESSAGES_PROMPT_H
#define CLIENT_MESSAGES_PROMPT_H

#include <vector>

#include "../models/Message.h"
namespace Client
{

    class MessagesPrompt
    {
    public:
        std::vector<Message> get();

    private:
        std::vector<Message> internal_get();
    };
}

#endif  // CLIENT_MESSAGES_PROMPT_H
