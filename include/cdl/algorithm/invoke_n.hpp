// invoke_n.hpp
// Free Function Implementation: invoke_n()
//
// A simple helper function to execute a callable a specified number of times.

#include <functional>

namespace cdl::algorithm
{
    void invoke_n(unsigned long n, std::function<void()> fn)
    {
        for (unsigned long i = 0; i < n; ++i)
        {
            fn();
        }
    }
}