// unique_ptr.hpp
// Class Implementation: cdl::memory::unique_ptr
//
// Imitates semantics of std::unique_ptr.

// unique_ptr.cpp
// A simple implementation of std::unqiue_ptr.

#pragma once

#include <utility>

namespace cdl::memory
{
    template<typename T>
    class unique_ptr
    {
        T* m_ptr;

    public:
        unique_ptr() = default;
        
        unique_ptr(T* p) 
            : m_ptr{ p } 
        {}
        
        ~unique_ptr()
        {
            if (m_ptr)
            {
                delete m_ptr;
            }
        }

        unique_ptr(unique_ptr const&)            = delete;
        unique_ptr& operator=(unique_ptr const&) = delete;

        unique_ptr(unique_ptr&& other)
            : m_ptr { other.m_ptr }
        {
            other.m_ptr = nullptr;
        }

        unique_ptr& operator=(unique_ptr&& other)
        {
            if (m_ptr)
            {
                delete m_ptr;
            }

            m_ptr = other.m_ptr;
            other.m_ptr = nullptr;

            return *this;
        }

        T operator*() const noexcept
        {
            return *m_ptr;
        }

        T* operator->() const noexcept
        {
            return m_ptr;
        }

        operator bool() const noexcept
        {
            return m_ptr != nullptr;
        }

        T* get() const noexcept
        {
            return m_ptr;
        }
    };

    template<typename T, typename... Args>
    unique_ptr<T> make_unique(Args... args)
    {
        return unique_ptr<T>{ new T{ args... } };
    }
}

