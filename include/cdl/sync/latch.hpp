// sync/latch.hpp
// A single-use thread sycnhronization barrier.

#ifndef CDL_LATCH_HPP
#define CDL_LATCH_HPP

#include <mutex>
#include <cassert>
#include <condition_variable>

namespace cdl::sync
{
    class latch
    {
        std::ptrdiff_t count;

        std::mutex              lock;
        std::condition_variable cv;

    public:
        explicit latch(std::ptrdiff_t expected)
            : count{expected}
            , lock{}
            , cv{} 
            {}

        ~latch() = default;

        // latch is non-copyable
        latch(latch const&)            = delete;
        latch& operator=(latch const&) = delete;
        
        // latch is non-movable
        latch(latch&&)            = delete;
        latch& operator=(latch&&) = delete;

        // atomically decrements the internal latch counter
        // by n without blocking the calling thread
        void count_down(std::ptrdiff_t n = 1)
        {
            {
                std::scoped_lock guard{lock};

                assert(n <= count);
                count -= n;
            }

            // wake blocked threads
            if (0 == count)
            {
                cv.notify_all();
            }
        }

        // test if the internal latch counter has reached 0
        // without blocking the caclling thread or decrementing
        [[nodiscard]]
        bool try_wait()
        {
            std::scoped_lock guard{lock};
            return (0 == count);
        }

        // block the calling thread until the internal counter reaches 0
        void wait()
        {
            std::unique_lock guard{lock};

            cv.wait(guard, 
                [this]() -> bool { return (0 == this->count); });
        }

        // atomically decrements the internal latch counter by n
        // and (if necessary) blocks the calling thread until the
        // internal latch counter reaches 0
        void arrive_and_wait(std::ptrdiff_t n = 1)
        {
            std::unique_lock guard{lock};

            assert(n <= count);

            count -= n;

            if (0 == count)
            {
                guard.unlock();
                cv.notify_all();
            }
            else
            {
                cv.wait(guard,
                    [this]() -> bool { return (0 == this->count); });
            }
        }
    };
}

#endif // CDL_LATCH_HPP