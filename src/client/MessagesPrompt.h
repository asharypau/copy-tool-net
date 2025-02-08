#ifndef FILE_PATH_PROMPT_H
#define FILE_PATH_PROMPT_H

#include <vector>

#include "../models/Message.h"

class MessagesPrompt
{
public:
    std::vector<Message> get();

private:
    std::vector<Message> internal_get();
};

#endif  // FILE_PATH_PROMPT_H
