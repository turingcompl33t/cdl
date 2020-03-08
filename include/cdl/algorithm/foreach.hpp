// foreach.hpp
// Free Function Implementation: cdl::algorithm::foreach()
//
// Basic implementation in the spirit of std::for_each()

#include <functional>

namespace cdl::algorithm
{
    template<typename Iter, typename Callable, typename... Args>
    void foreach(
        Iter begin, 
        Iter end, 
        Callable op, 
        Args const&... args
        )
    {
        auto current = begin;
        while (current != end)
        {
            std::invoke(op, args..., *current);
            ++current;
        }
    }
}