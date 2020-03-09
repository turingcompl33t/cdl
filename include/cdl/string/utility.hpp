// utility.hpp
// Free Function Implementation:
//  - cdl::string::length()

#pragma once

#include <wchar.h>
#include <cstring>
#include <cstddef>

namespace cdl::string::detail
{
    // string_length

    template <typename Char>
    std::size_t string_length(Char const* str);

    template <>
    std::size_t string_length(char const* str)
    {
        return strlen(str);
    }

    template <>
    std::size_t string_length(wchar_t const* str)
    {
        return wcslen(str);
    }

    // string_copy

    template <typename Char>
    Char* string_copy(Char* dst, Char const* src, std::size_t count);

    template<>
    char* string_copy(char* dst, char const* src, std::size_t count)
    {
        return strncpy(dst, src, count);
    }

    template <>
    wchar_t* string_copy(wchar_t* dst, wchar_t const* src, std::size_t count)
    {
        return wcsncpy(dst, src, count);
    }

    // string_compare

    template <typename Char>
    bool string_compare(Char const* left, Char const* right);

    template <>
    bool string_compare(char const* left, char const* right)
    {
        return (strcmp(left, right) == 0);
    }
    
    template <>
    bool string_compare(wchar_t const* left, wchar_t const* right)
    {
        return (wcscmp(left, right) == 0);
    }
}