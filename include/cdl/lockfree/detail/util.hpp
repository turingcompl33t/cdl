// lockfree/detail/util.hpp

#ifndef CDL_LOCKFREE_DETAIL_UTIL_HPP
#define CDL_LOCKFREE_DETAIL_UTIL_HPP

#include <cstddef>

namespace cdl::lockfree::detail
{
    // https://stackoverflow.com/questions/466204/rounding-up-to-next-power-of-2
    static std::size_t next_power_of_two_32(std::size_t v)
    {
        v--;
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;
        v |= v >> 16;
        return ++v;
    }

    // https://stackoverflow.com/questions/466204/rounding-up-to-next-power-of-2
    static std::size_t next_power_of_two_64(std::size_t v)
    {
        v--;
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;
        v |= v >> 16;
        v |= v >> 32;
        return ++v;
    }

    // Compute the next greater power of 2.
    std::size_t next_power_of_two(std::size_t v)
    {
        return (sizeof(std::size_t) == 8) 
            ? next_power_of_two_64(v) 
            : next_power_of_two_32(v);
    }
}

#endif // CDL_LOCKFREE_DETAIL_UTIL_HPP