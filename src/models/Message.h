#ifndef FILE_INFO_H
#define FILE_INFO_H

#include <boost/functional/hash.hpp>
#include <cstddef>
#include <string>

class Message
{
public:
    Message(size_t size_, std::string name_, std::string path_)
        : id(generate_id(size_, name_)),
          size(size_),
          name(name_),
          path(path_)
    {
    }

    size_t id;
    size_t size;
    std::string name;
    std::string path;

private:
    size_t generate_id(size_t size, const std::string& name)
    {
        size_t seed = 0;
        boost::hash_combine(seed, size);
        boost::hash_combine(seed, name);

        return seed;
    }
};

#endif  // FILE_INFO_H