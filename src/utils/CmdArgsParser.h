#ifndef CMD_ARG_PARSER_H
#define CMD_ARG_PARSER_H

#include <array>
#include <string>
#include <string_view>
#include <type_traits>

#include "CmdArgs.h"

class CmdArgsParser
{
public:
    /**
     * @brief Parses the command line arguments.
     *
     * This method takes the command line arguments and parses them into a CmdArgs object.
     *
     * @param argc The number of command line arguments.
     * @param argv The array of command line arguments.
     * @return CmdArgs The parsed command line arguments.
     */
    CmdArgs parse(int argc, char* argv[]);

private:
    /**
     * @brief Parses a given argument string and extracts a value based on specified tokens.
     *
     * This method searches for any of the specified tokens within the provided argument string.
     * If a token is found, it extracts the substring following a delimiter and converts it to
     * the TResult type. The conversion supports integral types, enums, and strings.
     *
     * @tparam TResult The type of the result to be extracted from the argument string.
     * @tparam Size The size of the tokens array.
     * @param arg The argument string to be parsed.
     * @param tokens An array of strings representing the tokens to search for in the argument string.
     * @return TResult The extracted value converted to the TResult type.
     */
    template <class TResult, std::size_t Size>
    TResult get(const std::string_view arg, const std::array<std::string, Size> tokens) const
    {
        TResult output;

        for (size_t i = 0; i < tokens.size(); i++)
        {
            if (arg.find(tokens[i]) != std::string::npos)
            {
                const std::size_t delimiter_pos = arg.find(delimiter);
                const std::string_view substr = arg.substr(delimiter_pos + 1);

                if constexpr (std::is_integral_v<TResult> || std::is_enum_v<TResult>)
                {
                    int val = std::stoi(std::string(substr));
                    output = static_cast<TResult>(val);
                }
                else if constexpr (std::is_same_v<TResult, std::string>)
                {
                    output = substr;
                }

                break;
            }
        }

        return output;
    }

    constexpr static const char delimiter = '=';
    const std::array<std::string, 2> port_tokens = {"port=", "p="};
    const std::array<std::string, 2> host_tokens = {"host=", "h="};
    const std::array<std::string, 4> app_type_tokens = {"application_type=", "app_type=", "type=", "at="};
};

#endif  // CMD_ARG_PARSER_H
