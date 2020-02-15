// barrier.hpp
// Class Implementation: cdl::synchronization::barrier
//
// A minimal barrier implementation; useful when one does not
// have access to boost::barrier or C++20 compiler.

#include <mutex>
#include <condition_variable>

namespace cdl::synchronization
{
    class barrier
    {
        std::mutex              mutex_;
        std::condition_variable cv_;

        unsigned counter_;
        unsigned waiting_;
        unsigned thread_count_;

    public:
        barrier(unsigned count)
            : thread_count_(count)
            , counter_(0)
            , waiting_(0)
        {}

        void wait()
        {
            std::unique_lock<std::mutex> lock(mutex_);
            ++counter_;
            ++waiting_;

            cv_.wait(lock, [&] {return counter_ >= thread_count_; });
            
            cv_.notify_one();
            --waiting_;

            if (waiting_ == 0)
            {
                // reset the barrier
                counter_ = 0;
            }

            lock.unlock();
        }
    };
}

