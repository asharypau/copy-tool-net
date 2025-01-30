#ifndef FILE_PATH_PROMPT_H
#define FILE_PATH_PROMPT_H

#include <string>
#include <vector>

class FilePathPrompt
{
public:
    std::vector<std::string> get();

private:
    bool exists(const std::string& file_name);
};

#endif  // FILE_PATH_PROMPT_H
