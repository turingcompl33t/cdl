// test/spin_latch.cpp

#include <catch2/catch.hpp>

#include <atomic>
#include <vector>
#include <future>
#include <cdl/sync/spin_latch.hpp>

constexpr static std::size_t const N_THREADS = 5;

static std::size_t worker(
    cdl::sync::spin_latch&    latch,
    std::atomic<std::size_t>& counter)
{
    counter.fetch_add(1);
    latch.arrive_and_wait();

    auto const observed = counter.load();
    return observed;
}

TEST_CASE("cdl::sync::spin_latch synchronizes thread activity")
{
    std::atomic<std::size_t> counter{0};

    cdl::sync::spin_latch latch{N_THREADS};

    std::vector<std::future<std::size_t>> futures{};
    for (auto i = 0ul; i < N_THREADS; ++i)
    {
        futures.emplace_back(
            std::async(
                std::launch::async, 
                worker, std::ref(latch), 
                std::ref(counter)));
    }

    for (auto& fut : futures)
    {
        auto const observed = fut.get();
        REQUIRE(observed == N_THREADS);
    }
}