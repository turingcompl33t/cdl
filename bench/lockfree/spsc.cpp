// bench/spsc/spsc.cpp

#include "benchmark/benchmark.h"

#include <chrono>
#include <thread>
#include <atomic>

#include <cdl/lockfree/spsc.hpp>

using namespace cdl;

constexpr static int64_t const MIN_OPS = 1 << 20;
constexpr static int64_t const MAX_OPS = 1 << 26;

static void BM_spsc_ping_pong_st(benchmark::State& state)
{
    using hr_clock = std::chrono::high_resolution_clock;

    auto const work_count = static_cast<std::size_t>(state.range(0));

    for (auto _ : state)
    {
        lockfree::spsc<std::size_t> queue{2};

        auto const start = hr_clock::now();

        std::size_t record{};
        for (auto i = 0ul; i < work_count; ++i)
        {
            queue.write(i);
            queue.read(record);
        }

        auto const stop = hr_clock::now();
        auto const elapsed = std::chrono::duration_cast<
            std::chrono::duration<double>>(stop - start);

        state.SetIterationTime(elapsed.count());
        state.SetItemsProcessed(static_cast<int64_t>(work_count));
    }
}

static void producer(
    lockfree::spsc<std::size_t>& queue, 
    std::atomic<bool>&           release_flag,
    std::size_t const            work_count)
{
    while (!release_flag.load(std::memory_order_acquire))
    {
        ;
    }

    std::size_t push_count{0};
    while (push_count < work_count)
    {
        if (queue.write(push_count))
        {
            ++push_count;
        }
    }
}

static void consumer(
    lockfree::spsc<std::size_t>& queue, 
    std::atomic<bool>&           release_flag,
    std::size_t const            work_count)
{
    while (!release_flag.load(std::memory_order_acquire))
    {
        ;
    }

    std::size_t record{};
    std::size_t pop_count{0};
    while (pop_count < work_count)
    {
        if (queue.read(record))
        {
            ++pop_count;
        }
    }
}

static void BM_spsc_ping_pong_mt(benchmark::State& state)
{
    using hr_clock = std::chrono::high_resolution_clock;

    auto const work_count = static_cast<std::size_t>(state.range(0));

    for (auto _ : state)
    {
        // NOTE: the capacity of the queue specified here has a
        // significant impact on the number of failed operations
        // that we experience during the test e.g. the number of
        // push() operations that fail on a full buffer and the
        // number of pop() operations that fail on an empty buffer
        lockfree::spsc<std::size_t> queue{64};

        std::atomic<bool> release_flag{false};
        auto producer_thread = std::thread{
            producer, std::ref(queue), std::ref(release_flag), work_count};
        auto consumer_thread = std::thread{
            consumer, std::ref(queue), std::ref(release_flag), work_count};

        auto const start = hr_clock::now();

        // release the threads
        release_flag.store(true, std::memory_order_release);

        // wait for them to complete
        producer_thread.join();
        consumer_thread.join();

        auto const stop = hr_clock::now();
        auto const elapsed = std::chrono::duration_cast<
            std::chrono::duration<double>>(stop - start);

        state.SetIterationTime(elapsed.count());
        state.SetItemsProcessed(static_cast<int64_t>(work_count));
    }
}

BENCHMARK(BM_spsc_ping_pong_st)->RangeMultiplier(2)->Range(MIN_OPS, MAX_OPS)->UseManualTime();
BENCHMARK(BM_spsc_ping_pong_mt)->RangeMultiplier(2)->Range(MIN_OPS, MAX_OPS)->UseManualTime();

BENCHMARK_MAIN();