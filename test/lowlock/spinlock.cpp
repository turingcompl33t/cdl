// test/lowlock/spinlock.cpp

#include <catch2/catch.hpp>

#include <vector>
#include <thread>

#include <cdl/lowlock/detail/spinlock.hpp>

constexpr static auto const N_THREADS      = 10ul;
constexpr static auto const OPS_PER_THREAD = 10000ul;

TEST_CASE("cdl::lowlock::detail::spinlock provides mutual exclusion")
{
    using cdl::lowlock::detail::spinlock;

    std::size_t counter{0};

    spinlock lock{};

    std::vector<std::thread> threads{};
    for (auto i = 0; i < N_THREADS; ++i)
    {
        threads.emplace_back(
            [&lock, &counter]()
            {
                for (auto i = 0; i < OPS_PER_THREAD; ++i)
                {
                    lock.lock();
                    ++counter;
                    lock.unlock();
                }   
            });
    }

    for (auto& t : threads)
    {
        t.join();
    }

    REQUIRE(counter == N_THREADS*OPS_PER_THREAD);
}