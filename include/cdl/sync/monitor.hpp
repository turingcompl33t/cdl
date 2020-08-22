// monitor.hpp
//
// Minimal monitor pattern implementation.

#ifndef CDL_SYNC_MONITOR_HPP
#define CDL_SYNC_MONITOR_HPP

#include <mutex>

namespace cdl::sync
{
    template <typename T>
    class monitor
    {
        T          t;
        std::mutex lock;

    public:
        monitor(T t_ = T{})
            : t{t_}
            , lock{} {}

        template<typename F>
        auto operator()(F f) -> decltype(f(t))
        {
            std::scoped_lock<std::mutex> guard{lock};
            return f(t);
        }
    };
}

#endif // CDL_SYNC_MONITOR_HPP