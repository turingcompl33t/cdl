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
        std::size_t m_size;

    public:
        basic_string()
            : m_buffer{ nullptr }
            , m_size{ 0 }
        {}

        basic_string(Char const* str)
        {
            auto len = string_length(str);
            
            m_buffer = new Char[len + 1];
            
            string_copy(m_buffer, str, len);
            m_buffer[len] = '\0';

            m_size = len;
        }

        basic_string(basic_string const& other)
            : m_buffer{ new char[other.m_size] }
            , m_size{ other.m_size }
        {
            string_copy(m_buffer, other.m_buffer, m_size);
        }

        basic_string& operator=(basic_string const& rhs)
        {
            if (this == &rhs) return *this;
            
            auto new_buffer = new char[rhs.m_size];
            delete[] m_buffer;
            
            m_buffer = new_buffer;
            m_size   = rhs.m_size;
            
            string_copy(m_buffer, rhs.m_buffer, m_size);
            
            return *this;
        }

        basic_string(basic_string&& other) noexcept
            : m_buffer{ other.m_buffer }
            , m_size{ other.m_size }
        {
            other.m_buffer = nullptr;
            other.m_size   = 0;
        }

        basic_string& operator=(basic_string&& rhs) noexcept
        {
            if (this == &rhs) return *this;
            
            delete[] m_buffer;

            m_buffer = rhs.m_buffer;
            m_size   = rhs.m_size;

            rhs.m_buffer = nullptr;
            rhs.m_size   = 0;

            return *this;
        }

        ~basic_string()
        {
            delete[] m_buffer;
        }

        std::size_t size() const noexcept;
        std::size_t length() const noexcept;
        bool empty() const noexcept;
    };

    using string  = basic_string<char>;
    using wstring = basic_string<wchar_t>;

    template <typename Char>
    std::size_t basic_string<Char>::size() const noexcept
    {
        return m_size;
    }

    template <typename Char>
    std::size_t basic_string<Char>::length() const noexcept
    {
        return m_size;
    }

    template<typename Char>
    bool basic_string<Char>::empty() const noexcept
    {
        return (m_size == 0);
    }
}