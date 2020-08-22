// sync/spin_latch.hpp
// A single-use thread sycnhronization barrier.
//
// Interface equivalent to sync::latch but significantly
// lighter-weight at the cost of spin waits rather than
// true OS-level thread sleeps.

#ifndef CDL_SPIN_LATCH_HPP
#define CDL_SPIN_LATCH_HPP

#include <atomic>
#include <thread>
#include <cassert>
#include <cstdio>

namespace cdl::sync
{
    class spin_latch
    {
        std::atomic<std::ptrdiff_t> count;

    public:
        constexpr explicit spin_latch(std::ptrdiff_t expected)
            : count{expected} {}

        ~spin_latch() = default;

        // latch is non-copyable
        spin_latch(spin_latch const&)            = delete;
        spin_latch& operator=(spin_latch const&) = delete;
        
        // latch is non-movable
        spin_latch(spin_latch&&)            = delete;
        spin_latch& operator=(spin_latch&&) = delete;

        // atomically decrements the internal latch counter
        // by n without blocking the calling thread
        void count_down(std::ptrdiff_t n = 1)
        {
            auto const old_count = count.fetch_sub(n, std::memory_order_release);
            assert(old_count >= n);
        }

        // test if the internal latch counter has reached 0
        // without blocking the caclling thread or decrementing
        [[nodiscard]]
        bool try_wait() const
        {
            return (0 == count.load(std::memory_order_acquire));
        }

        // block the calling thread until the internal counter reaches 0
        void wait()
        {
            while (count.load(std::memory_order_acquire) > 0)
            {
                std::this_thread::yield();
            }
        }

        // atomically decrements the internal latch counter by n
        // and (if necessary) blocks the calling thread until the
        // internal latch counter reaches 0
        void arrive_and_wait(std::ptrdiff_t n = 1)
        {
            auto const old_count = count.fetch_sub(n, std::memory_order_release);
            assert(old_count >= n);

            while (count.load(std::memory_order_acquire) > 0)
            {
                std::this_thread::yield();
            }
        }
    };
}

#endif // CDL_SPIN_LATCH_HPP