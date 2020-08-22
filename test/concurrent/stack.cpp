// stack.cpp
// Unit Test: cdl::concurrent::stack

#include <catch2/catch.hpp>

#include <thread>
#include <atomic>

#include <cdl/concurrent/stack.hpp>
#include <cdl/thread/thread_group.hpp>

constexpr static auto const N_PRODUCERS     = 10ul;
constexpr static auto const N_CONSUMERS     = 10ul;
constexpr static auto const WORK_PER_THREAD = 1000ul;

void producer(
	cdl::concurrent::stack<int>& s,
	std::atomic_int&             count)
{
	for (auto i = 0u; i < WORK_PER_THREAD; ++i)
	{
		s.push(count++);
	}
}

void consumer(
	cdl::concurrent::stack<int>& s,
	std::atomic_int&             count)
{
	int ret;
	for (auto i = 0u; i < WORK_PER_THREAD; ++i)
	{
		s.pop(ret);
		++count;
	}
}

TEST_CASE("cdl::concurrent::stack behaves as expected in single-threaded context")
{
	using cdl::concurrent::stack;

	stack<int> s{};

	REQUIRE(s.is_empty_unsafe());
	
	s.push(1);

	REQUIRE_FALSE(s.is_empty_unsafe());

	// no contention, this call must succeed
	int ret;
	s.pop(ret);
	REQUIRE(ret == 1);

	REQUIRE(s.is_empty_unsafe());
}

TEST_CASE("cdl::concurrent::stack observes the expected LIFO ordering of push / pop operations")
{
	using cdl::concurrent::stack;

	stack<int> s{};

	for (auto i = 0; i < static_cast<int>(WORK_PER_THREAD); ++i)
	{
		s.push(i);
	}

	int popped{};
	for (auto i = static_cast<int>(WORK_PER_THREAD) - 1; i >= 0; --i)
	{
		s.pop(popped);
		REQUIRE(i == popped);
	}

	REQUIRE(s.is_empty_unsafe());
}

TEST_CASE("cdl::concurrent::stack behaves as expected in multi-threaded context")
{
	using cdl::concurrent::stack;
	using cdl::thread::thread_group;

	stack<int> s{};
	thread_group producers{};
	thread_group consumers{};

	std::atomic_int producer_count{0};
	std::atomic_int consumer_count{0};

	REQUIRE(s.is_empty_unsafe());

	for (auto i = 0ul; i < N_PRODUCERS; ++i)
	{
		producers.emplace_thread(
			producer, 
			std::ref(s), 
			std::ref(producer_count));
	}

	producers.join_all();

	for (auto i = 0ul; i < N_CONSUMERS; ++i)
	{
		consumers.emplace_thread(
			consumer, 
			std::ref(s), 
			std::ref(consumer_count));
	}

	consumers.join_all();

	REQUIRE(producer_count.load() == N_PRODUCERS*WORK_PER_THREAD);
	REQUIRE(consumer_count.load() == N_CONSUMERS*WORK_PER_THREAD);

	REQUIRE(s.is_empty_unsafe());
}