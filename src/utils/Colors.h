#ifndef COLORS_H
#define COLORS_H

#include <string_view>

namespace Colors
{
    static constexpr std::string_view RESET = "\033[0m";
    static constexpr std::string_view RED = "\033[31m";
    static constexpr std::string_view GREEN = "\033[32m";
    static constexpr std::string_view YELLOW = "\033[33m";
    static constexpr std::string_view WHITE = "\033[37m";
    static constexpr std::string_view BRIGHT_BLUE = "\033[94m";
}

#endif  // COLORS_H
