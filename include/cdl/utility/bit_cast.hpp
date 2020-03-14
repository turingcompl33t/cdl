// bit_cast.hpp
// Free Function Implementation: cdl::utility::bit_cast()
//
// A hand-jammed implementation of C++20s std::bit_cast.

#pragma once

#include <cstring>
#include <type_traits>

namespace cdl::utility
{
    template <typename To,
              typename From,
              typename = std::enable_if_t<
                (sizeof(To) == sizeof(From)) &&
                std::is_trivially_copyable_v<To> &&
                std::is_trivially_copyable_v<From>>>
    To bit_cast(From &src)
    {
        To dst{};
        std::memcpy(&dst, &src, sizeof(To));
        return dst;
    }
}