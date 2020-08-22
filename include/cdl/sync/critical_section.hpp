// critical_section.hpp
//
// Wrapper for Win32 CRITICAL_SECTION for use with guarded, shared_guarded.

#pragma once

#include <windows.h>

namespace cdl::synchronization
{
    class critical_section
    {  
        CRITICAL_SECTION m_lock;

    public:
        critical_section() : m_lock{}
        {
            ::InitializeCriticalSection(&m_lock);
        }

        critical_section(unsigned long const spin_count)
            : m_lock{}
        {
            ::InitializeCriticalSectionAndSpinCount(&m_lock, spin_count);
        }

        ~critical_section() = default;
        
        // non-copyable
        critical_section(critical_section const&)            = delete;
        critical_section& operator=(critical_section const&) = delete;

        // TODO: make movable
        critical_section(critical_section&&)            = delete;
        critical_section& operator=(critical_section&&) = delete;

        auto lock() -> void
        {
            ::EnterCriticalSection(&m_lock);
        }

        auto try_lock() -> bool
        {
            auto const r = ::TryEnterCriticalSection(&m_lock);
            return (TRUE == r);
        }

        auto unlock() -> void
        {
            ::LeaveCriticalSection(&m_lock);
        }
    };
};