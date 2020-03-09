// string.hpp
// Class Implementation: cdl::string::string
//
// A basic string class that implements the std::string API.

#pragma once

#include <cstring>
#include <cstddef>

#include "utility.hpp"

namespace cdl::string
{
    // cdl::string::basic_string
    //
    // Base string class template.

    template <typename Char>
    class basic_string
    {   
        Char*       m_buffer;
        std::size_t m_count;     // count of characters in string
        std::size_t m_capacity;  // size of underlying buffer (in characters)

    public:
        basic_string()
            : m_buffer{ nullptr }
            , m_count{ 0 }
            , m_capacity{ 0 }
        {}

        basic_string(Char const* str)
        {
            auto len = detail::string_length(str);
            
            m_buffer = new Char[len + 1];
            
            detail::string_copy(m_buffer, str, len);
            m_buffer[len] = '\0';

            m_count    = len;
            m_capacity = len + 1;
        }

        basic_string(basic_string const& other)
            : m_buffer{ new Char[other.m_capacity] }
            , m_count{ other.m_count }
            , m_capacity{ other.m_capacity }
        {
            detail::string_copy(m_buffer, other.m_buffer, m_count);
            m_buffer[m_count] = '\0';
        }

        basic_string& operator=(basic_string const& rhs)
        {
            if (this == &rhs) return *this;
            
            auto new_buffer = new char[rhs.m_capacity];
            delete[] m_buffer;
            
            m_buffer   = new_buffer;
            m_count    = rhs.m_count;
            m_capacity = rhs.m_capacity;
            
            detail::string_copy(m_buffer, rhs.m_buffer, m_count);
            m_buffer[m_count] = '\0';
            
            return *this;
        }

        basic_string(basic_string&& other) noexcept
            : m_buffer{ other.m_buffer }
            , m_count{ other.m_count }
            , m_capacity{ other.m_capacity }
        {
            other.m_buffer   = nullptr;
            other.m_count    = 0;
            other.m_capacity = 0;
        }

        basic_string& operator=(basic_string&& rhs) noexcept
        {
            if (this == &rhs) return *this;
            
            delete[] m_buffer;

            m_buffer   = rhs.m_buffer;
            m_count    = rhs.m_count;
            m_capacity = rhs.m_capacity;

            rhs.m_buffer   = nullptr;
            rhs.m_count    = 0;
            rhs.m_capacity = 0;

            return *this;
        }

        ~basic_string()
        {
            delete[] m_buffer;
        }

        std::size_t size() const noexcept;
        std::size_t length() const noexcept;
        bool empty() const noexcept;

        bool is_equal(basic_string const& other);
        bool is_equal(Char const* str);
    };

    using string  = basic_string<char>;
    using wstring = basic_string<wchar_t>;

    template <typename Char>
    std::size_t basic_string<Char>::size() const noexcept
    {
        return m_count;
    }

    template <typename Char>
    std::size_t basic_string<Char>::length() const noexcept
    {
        return m_count;
    }

    template<typename Char>
    bool basic_string<Char>::empty() const noexcept
    {
        return (m_count == 0);
    }

    template <typename Char>
    bool basic_string<Char>::is_equal(
        basic_string<Char> const& other
        )
    {
        return detail::string_compare(m_buffer, other.m_buffer);
    }

    template <typename Char>
    bool basic_string<Char>::is_equal(
        Char const* str
    )
    {
        return detail::string_compare(m_buffer, str);
    }
}