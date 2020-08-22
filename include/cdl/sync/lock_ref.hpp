// lock_ref.hpp
//
// A movable lock reference, similar to std::unique_lock.

#pragma once

#include <memory>  // std::addressof

namespace cdl::synchronization
{
    enum class lock_mode
    {
        try_lock,    // attempt to lock without blocking
        adopt_lock,  // assume caller already owns lock
        defer_lock   // construct without locking
    };

    template <typename M>
    class lock_ref
    {
        M*   m_lock;
        bool m_owns;

    public:
        lock_ref() 
            : m_lock{}
            , m_owns{false} 
        {}

        // contstruct and lock
        lock_ref(M& lock) 
            : m_lock{std::addressof(lock)}
            , m_owns{false}
        {
            m_lock->lock();
            m_owns = true;
        }

        // construct and manage lock with custom lock mode
        lock_ref(M& lock, lock_mode m)
            : m_lock{std::addressof(lock)}
            , m_owns{false}
        {
            auto updated_owns = false;

            switch (m)
            {
            case lock_mode::try_lock:  // try to lock without blocking
                updated_owns = m_lock->try_lock();
                break;
            case lock_mode::adopt_lock:  // assume caller owns lock
                updated_owns = true;
                break;
            case lock_mode::defer_lock:  // do no lock
            default:
                break;
            }

            m_owns = updated_owns;
        }

        ~lock_ref()
        {
            if (m_owns)
            {
                m_lock->unlock();
            }
        }

        // non-copyable
        lock_ref(lock_ref const&)            = delete;
        lock_ref& operator=(lock_ref const&) = delete;

        // move-constructible
        lock_ref(lock_ref&& other) noexcept 
            : m_lock{other.m_lock}
            , m_owns{other.m_owns}
        {
            other.m_lock = nullptr;
            other.m_owns = false;
        }

        // move-assignable
        lock_ref& operator=(lock_ref&& rhs)
        {
            if (this != std::addressof(rhs))
            {
                if (m_owns)
                {
                    m_lock->unlock();
                }

                m_lock = other.m_lock;
                m_owns = other.m_owns;

                other.m_lock = nullptr;
                other.m_owns = false;
            }

            return *this;
        }

        auto lock() -> void
        {
            // TODO: validation
            m_lock->lock();
            m_owns = true;
        } 

        auto try_lock() -> bool
        {
            // TODO: validation
            auto const acquired = m_lock->try_lock();
            m_owns = acquired;
            return acquired;
        }

        auto unlock() -> void
        {
            // TODO: validation
            m_lock->unlock();
            m_owns = false;
        }

        auto owns_lock() const noexcept -> bool
        {
            return m_owns;
        }

        // TODO: support for acquire/release of shared locks
    };
}