// guarded.hpp
// Class Implementation: cdl::guarded::guarded
//
// Object interface that presents a simple API that ensures thread safety.
//
// Inspired by the implementation by copperspice
//  https://github.com/copperspice/cs_libguarded

#pragma once

#include <mutex>
#include <memory>

namespace cdl::guarded
{
    // cdl::guarded::guarded
    //
    // Lock-based object wrapper for safe concurrent access.

    template <typename T, typename M = std::mutex>
    class guarded
    {
    private:
        class deleter;

    public:
        using handle = std::unique_ptr<T, deleter>;

        template <typename... Us>
        guarded(Us&&... args);

        handle lock();

        handle try_lock();

        template <typename Duration>
        handle try_lock_for(Duration const& duration);

        template <typename TimePoint>
        handle try_lock_until(TimePoint const& timepoint);

    private:
        T m_obj;
        M m_mutex;
    };

    // cdl::guarded::deleter
    //
    // Custom deleter for guarded instances that
    // releases the underlying unique_lock on destruction

    template<typename T, typename M>
    class guarded<T, M>::deleter
    {
    public:
        using pointer = T*;

        deleter(std::unique_lock<M> lock);

        void operator()(T*);

    private:
        std::unique_lock<M> m_lock;
    };

    template <typename T, typename M>
    guarded<T, M>::deleter::deleter(std::unique_lock<M> lock)
        : m_lock{std::move(lock)}
    {}

    template <typename T, typename M>
    void guarded<T, M>::deleter::operator()(T*)
    {
        

        if (m_lock.owns_lock())
        {
            m_lock.unlock();
        }
    }

    template<typename T, typename M>
    template <typename... Us>
    guarded<T, M>::guarded(Us&&... args)
        : m_obj{std::forward<Us>(args)...}
    {}

    template <typename T, typename M>
    auto guarded<T, M>::lock() -> handle
    {
        std::unique_lock<M> lock{m_mutex};
        return handle{&m_obj, deleter{std::move(lock)}};
    }

    template <typename T, typename M>
    auto guarded<T, M>::try_lock() -> handle
    {
        std::unique_lock<M> lock{m_mutex, std::try_to_lock};

        if (lock.owns_lock())
        {
            return handle{&m_obj, deleter{std::move(lock)}};
        }
        else
        {
            return handle{nullptr, deleter{std::move(lock)}};
        }
    }

    template <typename T, typename M>
    template <typename Duration>
    auto guarded<T, M>::try_lock_for(Duration const& duration) -> handle
    {
        std::unique_lock<M> lock{m_mutex, duration};

        if (lock.owns_lock())
        {
            return handle{&m_obj, deleter{std::move(lock)}};
        }
        else
        {
            return handle{nullptr, deleter{std::move(lock)}};
        }
    }
}