#include "utility.hpp"

namespace NUtility
{
    std::chrono::milliseconds getCurrentTime()
    {
        return std::chrono::duration_cast< std::chrono::milliseconds >(
        std::chrono::system_clock::now().time_since_epoch());
    }
} // namespace NUtility

