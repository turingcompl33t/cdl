// monitor.hpp
// Minimal monitor pattern implementation.

#include <mutex>

namespace cdl
{
    template <typename T>
    class monitor
    {
    private:
        T          t_;
        std::mutex mutex_;
    public:
        monitor(T t = T{})
            : t_{ t }
        {}

        template<typename F>
        auto operator()(F f) -> decltype(f(t_))
        {
            std::lock_guard<std::mutex> guard{ mutex_ };
            return f(t_);
        }
    };
}

