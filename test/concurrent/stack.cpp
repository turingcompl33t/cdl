// stack.cpp
// Unit Test: cdl::concurrent::stack

#include <catch2/catch.hpp>
#include <cdl/concurrent/stack.hpp>
#include <cdl/thread/thread_group.hpp>

#include <thread>
#include <atomic>

constexpr auto N_PRODUCERS  = 10;
constexpr auto N_CONSUMERS  = 10;
constexpr auto N_ITERATIONS = 1000;

void producer(
	cdl::concurrent::stack<unsigned>& s,
	std::atomic_uint&                 count
	)
{
	for (auto i = 0u; i < N_ITERATIONS; ++i)
	{
		s.push(count++);
	}
}

void consumer(
	cdl::concurrent::stack<unsigned>& s,
	std::atomic_uint&                 count
	)
{
	unsigned ret;
	for (auto i = 0u; i < N_ITERATIONS; ++i)
	{
		s.pop(ret);
		++count;
	}
}

TEST_CASE("cdl::concurrent::stack behaves as expected in single-threaded context")
{
	using cdl::concurrent::stack;

	stack<int> s{};

	REQUIRE(s.empty());
	
	s.push(1);

	REQUIRE_FALSE(s.empty());

	// no contention, this call must succeed
	int ret;
	s.pop(ret);
	REQUIRE(ret == 1);

	REQUIRE(s.empty());
}

TEST_CASE("cdl::concurrent::stack behaves as expected in multi-threaded context")
{
	using cdl::concurrent::stack;
	using cdl::thread::thread_group;

	stack<unsigned> s{};
	thread_group producers{};
	thread_group consumers{};

	std::atomic_uint producer_count{0};
	std::atomic_uint consumer_count{0};

	REQUIRE(s.empty());

	for (auto i = 0u; i < N_PRODUCERS; ++i)
	{
		producers.emplace_thread(producer, std::ref(s), std::ref(producer_count));
	}

	producers.join_all();

	for (auto i = 0u; i < N_CONSUMERS; ++i)
	{
		consumers.emplace_thread(consumer, std::ref(s), std::ref(consumer_count));
	}

	consumers.join_all();

	REQUIRE(producer_count.load() == N_PRODUCERS*N_ITERATIONS);
	REQUIRE(consumer_count.load() == N_CONSUMERS*N_ITERATIONS);
	REQUIRE(s.empty());
}