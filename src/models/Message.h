#ifndef FILE_INFO_H
#define FILE_INFO_H

#include <string>

struct Message
{
    Message(size_t size_, std::string name_, std::string path_)
        : size(size_),
          name(name_),
          path(path_)
    {
    }

    size_t size;
    std::string name;
    std::string path;
};

#endif  // FILE_INFO_H