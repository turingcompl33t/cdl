// guarded_test.cpp
// Unit Test: cdl::guarded::guarded
//
// Build
//	cl /EHsc /nologo /std:c++17 /W4 /I C:\Dev\CDL guarded_test.cpp

#include <queue>
#include <vector>
#include <cassert>
#include <iostream>

#include "cdl/guarded/guarded.hpp"
#include "cdl/synchronization/wait.hpp"

constexpr auto N_PRODUCERS = 10;
constexpr auto N_CONSUMERS = 10;
constexpr auto N_ITEMS     = 1000;

cdl::guarded::guarded<std::queue<int>> g_guarded_queue{};

void producer(unsigned id)
{
    for (auto i = 0; i < N_ITEMS; ++i)
    {
        auto handle = g_guarded_queue.lock();
        handle->emplace(id);
    }
}

void consumer()
{
    for (auto i = 0; i < N_ITEMS; i++)
    {
        auto handle = g_guarded_queue.lock();
        handle->pop();
    }
}

int main()
{
    std::vector<std::thread> producers{};
    std::vector<std::thread> consumers{};

    for (auto id = 0; id < N_PRODUCERS; ++id)
    {
        producers.emplace_back(producer, id);
    }

    cdl::synchronization::join_all(producers);

    for (auto i = 0; i < N_CONSUMERS; ++i)
    {
        consumers.emplace_back(consumer);
    }

    cdl::synchronization::join_all(consumers);

    auto handle = g_guarded_queue.lock();
    assert(handle->empty());
}  