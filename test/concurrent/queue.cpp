// queue.cpp
// Unit Test: cdl::concurrent::queue

#include <catch2/catch.hpp>
#include <cdl/concurrent/queue.hpp>
#include <cdl/thread/thread_group.hpp>

#include <thread>
#include <atomic>

constexpr auto N_PRODUCERS  = 10;
constexpr auto N_CONSUMERS  = 10;
constexpr auto N_ITERATIONS = 1000;

void producer(
	cdl::concurrent::queue<unsigned>& q,
	std::atomic_uint&                 count
	)
{
	for (auto i = 0u; i < N_ITERATIONS; ++i)
	{
		q.push(count++);
	}
}

void consumer(
	cdl::concurrent::queue<unsigned>& q,
	std::atomic_uint&                 count
	)
{
	unsigned ret;
	for (auto i = 0u; i < N_ITERATIONS; ++i)
	{
		q.wait_and_pop(ret);
		++count;
	}
}

TEST_CASE("cdl::concurrent::queue behaves as expected in single-threaded context")
{
	using cdl::concurrent::queue;

	queue<int> q{};

	REQUIRE(q.empty());
	
	q.push(1);

	REQUIRE_FALSE(q.empty());

	// no contention, this call must succeed
	int ret;
	REQUIRE(q.try_pop(ret));
	REQUIRE(ret == 1);

	REQUIRE(q.empty());
}

TEST_CASE("cdl::concurrent::queue behaves as expected in multi-threaded context")
{
	using cdl::concurrent::queue;
	using cdl::thread::thread_group;

	queue<unsigned> q{};
	thread_group producers{};
	thread_group consumers{};

	std::atomic_uint producer_count{0};
	std::atomic_uint consumer_count{0};

	REQUIRE(q.empty());

	for (auto i = 0u; i < N_PRODUCERS; ++i)
	{
		producers.emplace_thread(producer, std::ref(q), std::ref(producer_count));
	}

	for (auto i = 0u; i < N_CONSUMERS; ++i)
	{
		consumers.emplace_thread(consumer, std::ref(q), std::ref(consumer_count));
	}

	producers.join_all();
	consumers.join_all();

	REQUIRE(producer_count.load() == N_PRODUCERS*N_ITERATIONS);
	REQUIRE(consumer_count.load() == N_CONSUMERS*N_ITERATIONS);
	REQUIRE(q.empty());
}