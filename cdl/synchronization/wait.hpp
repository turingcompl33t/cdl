// wait.hpp
// Free Function Implementations:
//  - join_all()

namespace cdl::synchronization
{
    template <typename T>
    void join_all(T& container)
    {
        for (auto& t : container)
        {
            if (t.joinable())
            {
                t.join();
            }
        }
    }
}