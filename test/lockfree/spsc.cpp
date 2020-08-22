// test/lockfree/spsc.cpp

#include <catch2/catch.hpp>

#include <future>

#include <cdl/lockfree/spsc.hpp>

constexpr static std::size_t const N_RECORDS = 100;

static std::size_t producer(cdl::lockfree::spsc<int>& queue)
{
    int value{117};
    
    std::size_t produced{0};
    while (produced < N_RECORDS)
    {
        if (queue.write(value))
        {
            ++produced;
        }
    }

    return produced;
}

static std::size_t consumer(cdl::lockfree::spsc<int>& queue)
{
    int value{};

    std::size_t consumed{0};
    while (consumed < N_RECORDS)
    {
        if (queue.read(value))
        {
            ++consumed;
        }
    }

    return consumed;
}

TEST_CASE("cdl::lockfree::spsc appears to function properly")
{
    using namespace cdl;

    lockfree::spsc<int> queue{16};

    auto p = std::async(std::launch::async, producer, std::ref(queue));
    auto c = std::async(std::launch::async, consumer, std::ref(queue));

    auto const produced = p.get();
    auto const consumed = c.get();

    REQUIRE(produced == N_RECORDS);
    REQUIRE(consumed == N_RECORDS);

    REQUIRE(queue.is_empty_unsafe());
}