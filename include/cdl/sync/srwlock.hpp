// srwlock.hpp
//
// Wrapper around Win32 SRWLOCK for use with guarded, shared_guarded

#pragma once

#include <windows.h>

namespace cdl::synchronization
{
    class srwlock
    {
        SRWLOCK m_lock;

    public:
        srwlock() : m_lock{}
        {
            ::InitializeSRWLock(&m_lock);
        }

        ~srwlock() = default;
        
        // non-copyable
        srwlock(srwlock const&)            = delete;
        srwlock& operator=(srwlock const&) = delete;

        // TODO: make movable
        srwlock(srwlock&&)            = delete;
        srwlock& operator=(srwlock&&) = delete;

        auto lock() -> void
        {
            ::AcquireSRWLockExclusive(&m_lock);
        }

        auto try_lock() -> bool
        {
            auto const r = ::TryAcquireSRWLockExclusive(&m_lock);
            return (TRUE == r);
        }

        auto unlock() -> void
        {
            ::ReleaseSRWLockExclusive(&m_lock);
        }

        auto lock_shared() -> void
        {
            ::AcquireSRWLockShared(&m_lock);
        }

        auto try_lock_shared() -> bool
        {
            auto const r = ::TryAcquireSRWLockShared(&m_lock);
            return (TRUE == r);
        }

        auto unlock_shared() -> void
        {
            ::ReleaseSRWLockShared(&m_lock);
        }
    };
}