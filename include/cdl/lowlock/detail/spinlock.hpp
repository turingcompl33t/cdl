// spinlock.hpp

#ifndef CDL_LOWLOCK_SPINLOCK_HPP
#define CDL_LOWLOCK_SPINLOCK_HPP

#include <atomic>
#include <stdexcept>

namespace cdl::lowlock::detail
{
    class spinlock
    {
        std::atomic_bool locked;

    public:
        spinlock() : locked{false} {}

        spinlock(spinlock const&)            = delete;
        spinlock& operator=(spinlock const&) = delete;

        spinlock(spinlock&&)            = delete;
        spinlock& operator=(spinlock&&) = delete;

        auto lock() -> void
        {
            // acquire ownership of the lock
            while (std::atomic_exchange(&locked, true)) {}
        }

        auto unlock() -> void
        {
            if (!std::atomic_load(&locked))
            {
                throw std::runtime_error{"Attempt to unlock non-owned spinlock"};
            }

            // release ownership of the lock
            std::atomic_store(&locked, false);
        }
    };
}

#endif // CDL_LOWLOCK_SPINLOCK_HPP