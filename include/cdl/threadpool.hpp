// threadpool.hpp
// Convenience Header.

#include "threadpool/thread_pool_local.hpp"

namespace cdl::threadpool
{
    // only expose the most advanced implementation from header
    using threadpool = cdl::threadpool::thread_pool_local;
}
