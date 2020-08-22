// lockfree/detail/cache_line.hpp

#ifndef CDL_LOCKFREE_DETAIL_CACHE_LINE_HPP
#define CDL_LOCKFREE_DETAIL_CACHE_LINE_HPP

#include <cstdint>

namespace cdl::lockfree::detail
{
    struct cache_line
    {
        constexpr static std::size_t const size{64};
    };
}

#endif // CDL_LOCKFREE_DETAIL_CACHE_LINE_HPP