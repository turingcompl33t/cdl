// thread_group.hpp
// A logical set of threads that may be managed collectively.

#ifndef CDL_THREAD_THREAD_GROUP
#define CDL_THREAD_THREAD_GROUP

#include <vector>
#include <mutex>
#include <thread>

namespace cdl::thread
{    
    class thread_group
    {
        mutable std::mutex       lock;
        std::vector<std::thread> threads;

    public:
        thread_group() = default;
        
        ~thread_group()
        {
            // automatically joins all threads in the group on destruction
            join_all();
        }

        thread_group(thread_group const&)            = delete;
        thread_group& operator=(thread_group const&) = delete;

        thread_group(thread_group&&)            = delete;
        thread_group& operator=(thread_group&&) = delete;

        template <typename Func, typename... Args>
        void emplace_thread(Func&& fn, Args&&... args)
        {
            std::scoped_lock<std::mutex> guard{lock};
            threads.emplace_back(std::forward<Func>(fn), std::forward<Args>(args)...);
        }

        void add_thread(std::thread&& t)
        {
            std::scoped_lock guard{lock};
            threads.push_back(std::move(t));
        }

        void join_all()
        {
            std::scoped_lock guard{lock};
            for (auto& t: threads)
            {
                if (t.joinable())
                {
                    t.join();
                }
            }
        }

        std::size_t size() const noexcept
        {
            std::scoped_lock guard{lock};
            return threads.size();
        }
    };
}

#endif // CDL_THREAD_THREAD_GROUP