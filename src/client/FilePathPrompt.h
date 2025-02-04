#ifndef FILE_PATH_PROMPT_H
#define FILE_PATH_PROMPT_H

#include <string>
#include <utility>
#include <vector>

class FilePathPrompt
{
public:
    std::vector<std::pair<std::string, std::string>> get();

private:
    std::vector<std::pair<std::string, std::string>> internal_get();
};

#endif  // FILE_PATH_PROMPT_H
