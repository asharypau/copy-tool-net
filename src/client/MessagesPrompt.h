#ifndef MESSAGES_PROMPT_H
#define MESSAGES_PROMPT_H

#include <vector>

#include "../models/Message.h"

class MessagesPrompt
{
public:
    std::vector<Message> get();

private:
    std::vector<Message> internal_get();
};

#endif  // MESSAGES_PROMPT_H
