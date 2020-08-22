// lockfree/detail/cache_pad.hpp

#ifndef CDL_LOCKFREE_DETAIL_CACHE_PAD_HPP
#define CDL_LOCKFREE_DETAIL_CACHE_PAD_HPP

#include <cstddef>
#include <cstdint>

#include "cache_line.hpp"

namespace cdl::lockfree::detail
{
    template <std::size_t N>
    class cache_pad
    {
        static_assert(N <= cache_line::size,
            "Cache pad should be smalled than a cache line");
        uint8_t pad[N];
    };
}

#endif // CDL_LOCKFREE_DETAIL_CACHE_PAD_HPP