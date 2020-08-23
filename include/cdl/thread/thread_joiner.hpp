// thread/thread_joiner.hpp

#ifndef CDL_THREAD_THREAD_JOINER_HPP
#define CDL_THREAD_THREAD_JOINER_HPP

#include <vector>
#include <thread>

namespace cdl::thread
{
    class thread_joiner
    {
        std::vector<std::thread>& threads;
    public:
        explicit thread_joiner(std::vector<std::thread>& threads_)
            : threads{threads_} {}

        ~thread_joiner()
        {
            for (auto& t : threads)
            {
                if (t.joinable())
                {
                    t.join();
                }
            }
        }

        thread_joiner(thread_joiner const&)            = delete;
        thread_joiner& operator=(thread_joiner const&) = delete;

        thread_joiner(thread_joiner&&)            = delete;
        thread_joiner& operator=(thread_joiner&&) = delete;
    };
}

#endif // CDL_THREAD_THREAD_JOINER_HPP