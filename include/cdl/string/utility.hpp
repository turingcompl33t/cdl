// utility.hpp
// Free Function Implementation:
//  - cdl::string::length()

#pragma once

#include <wchar.h>
#include <cstring>
#include <cstddef>

namespace cdl::string
{
    std::size_t string_length(char const* str)
    {
        return std::strlen(str);
    }

    std::size_t string_length(wchar_t const* str)
    {
        return wcslen(str);
    }

    char* string_copy(char* dst, char const* src, std::size_t count)
    {
        return std::strncpy(dst, src, count);
    }

    wchar_t* string_copy(wchar_t* dst, wchar_t const* src, std::size_t count)
    {
        return wcsncpy(dst, src, count);
    }
}