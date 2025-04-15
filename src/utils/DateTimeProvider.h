#ifndef DATA_TIME_PROVIDER
#define DATA_TIME_PROVIDER

#include <chrono>
#include <string>

class DateTimeProvider
{
public:
    static std::string utc_now()
    {
        auto now = std::chrono::system_clock::now();
        std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);

        std::tm utc_tm{};
        gmtime_s(&utc_tm, &now_time_t);

        std::ostringstream oss;
        oss << std::put_time(&utc_tm, "%Y-%m-%dT%H:%M:%SZ");

        return oss.str();
    }
};

#endif  // DATA_TIME_PROVIDER
