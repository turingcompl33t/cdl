// guarded.hpp
//
// Interface inspired by std::sync::Mutex from the Rust stdlib.
// Name inspired by guarded<T> from cs_libguarded by CopperSpice.
//
// NOTE: implementation is Windows-specific (uses Win32 synchronization primitives)

#pragma once

#include <windows.h>

#include <memory>
#include <utility>

#include "lock_ref.hpp"

namespace cdl::synchronization
{
    template <typename T, typename M>
    class guarded
    {
        T m_obj;
        M m_lock;

        class deleter;

    public:
        using handle = std::unique_ptr<T, deleter>;

        guarded(T obj) 
            : m_obj{obj}
            , m_lock{} 
        {}
        
        template<typename... Args>
        guarded(Args&&... args) 
            : m_obj{std::forward<Args>(args)...}
            , m_lock{} 
        {}

        ~guarded() = default;

        guarded(guarded const&)            = delete;
        guarded& operator=(guarded const&) = delete;

        [[nodiscard]]
        auto lock() -> handle
        {
            auto ref = lock_ref<M>{m_lock};
            return handle{&m_obj, deleter{std::move(ref)}};
        }

        [[nodiscard]]
        auto try_lock() -> handle
        {
            auto ref = lock_ref{m_lock, lock_mode::try_lock};
            if (ref.owns_lock())
            {
                return handle{&m_obj, deleter{std::move(ref)}};
            }
            
            return handle{nullptr, deleter(std::move(ref))};
        }

        // TODO: try_lock_for()

        // TODO: try_lock_until()
    };

    template <typename T, typename M>
    class guarded<T, M>::deleter
    {
        lock_ref<M> m_lock;

    public:
        using pointer = T*;

        deleter(lock_ref<M> ref) : m_lock{std::move(ref)} {}

        auto operator()(T*) -> void
        {
            if (m_lock.owns_lock())
            {
                m_lock.unlock();
            }
        }
    };
}