#ifndef FILE_INFO_H
#define FILE_INFO_H

#include <boost/functional/hash.hpp>
#include <string>

class Message
{
public:
    Message(std::size_t size_, std::string name_, std::string path_)
        : id(generate_id(size_, name_)),
          size(size_),
          name(name_),
          path(path_)
    {
    }

    std::size_t id;
    std::size_t size;
    std::string name;
    std::string path;

private:
    std::size_t generate_id(std::size_t size, const std::string& name)
    {
        std::size_t seed = 0;
        boost::hash_combine(seed, size);
        boost::hash_combine(seed, name);

        return seed;
    }
};

#endif  // FILE_INFO_H