// thread_group.hpp
// Class Implementation: cdl::thread::thread_group

#pragma once

#include <list>
#include <mutex>
#include <thread>

namespace cdl::thread
{
    // cdl::thread::thread_group
    //
    // A logical set of threads that may be managed collectively.

    class thread_group
    {
        std::list<std::thread> m_threads;
        mutable std::mutex     m_mutex;

    public:
        thread_group() = default;
        
        ~thread_group()
        {
            join_all();
        }

        template <typename Func, typename... Args>
        void emplace_thread(Func&& fn, Args&&... args)
        {
            // QUESTION: why does class template argument deduction
            // fail for this scoped_lock instance but succeed for the others?
            // obviously has something to do with the fact that this
            // is a function template whereas the other members are not
            std::scoped_lock<std::mutex> guard{m_mutex};
            m_threads.emplace_back(std::forward<Func>(fn), std::forward<Args>(args)...);
        }

        void add_thread(std::thread&& t)
        {
            std::scoped_lock guard{m_mutex};
            m_threads.push_back(std::move(t));
        }

        void join_all()
        {
            std::scoped_lock guard{m_mutex};
            for (auto& t: m_threads)
            {
                if (t.joinable())
                {
                    t.join();
                }
            }
        }

        std::size_t count() const noexcept
        {
            std::scoped_lock guard{m_mutex};
            return m_threads.size();
        }
    };


}