// lowlock/cache_line.hpp

#ifndef CDL_LOWLOCK_CACHE_LINE_HPP
#define CDL_LOWLOCK_CACHE_LINE_HPP

#include <cstdint>

namespace cdl::lowlock::detail
{
    struct cache_line
    {
        constexpr static std::size_t const size{64};
    };
}

#endif // CDL_LOWLOCK_CACHE_LINE_HPP