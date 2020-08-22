// test/queue.cpp

#include <catch2/catch.hpp>

#include <thread>
#include <atomic>

#include <cdl/concurrent/queue.hpp>
#include <cdl/thread/thread_group.hpp>

constexpr static auto const N_PRODUCERS     = 10ul;
constexpr static auto const N_CONSUMERS     = 10ul;
constexpr static auto const WORK_PER_THREAD = 1000;

static void producer(
	cdl::concurrent::queue<int>& q,
	std::atomic_int&             count)
{
	for (auto i = 0ul; i < WORK_PER_THREAD; ++i)
	{
		q.push(count++);
	}
}

static void consumer(
	cdl::concurrent::queue<int>& q,
	std::atomic_int&             count
	)
{
	int ret;
	for (auto i = 0ul; i < WORK_PER_THREAD; ++i)
	{
		q.wait_and_pop(ret);
		++count;
	}
}

TEST_CASE("cdl::concurrent::queue behaves as expected in single-threaded context")
{
	using cdl::concurrent::queue;

	queue<int> q{};

	REQUIRE(q.is_empty_unsafe());
	
	q.push(1);

	REQUIRE_FALSE(q.is_empty_unsafe());

	// no contention, this call must succeed
	int ret;
	REQUIRE(q.try_pop(ret));
	REQUIRE(ret == 1);

	REQUIRE(q.is_empty_unsafe());
}

TEST_CASE("cdl::concurrent::queue observes the expected FIFO ordering of push / pop operations")
{
	using cdl::concurrent::queue;

	queue<int> q{};

	// push [0 .. WORK_PER_THREAD - 1] onto queue 
	for (auto i = 0; i < static_cast<int>(WORK_PER_THREAD); ++i)
	{
		q.push(i);
	}

	// pop the values off the queue and ensure they are in the correct order
	int popped{};
	for (auto i = 0; i < static_cast<int>(WORK_PER_THREAD); ++i)
	{
		q.try_pop(popped);
		REQUIRE((i == popped));
	}

	REQUIRE(q.is_empty_unsafe());
}

TEST_CASE("cdl::concurrent::queue behaves as expected in multi-threaded context")
{
	using cdl::concurrent::queue;
	using cdl::thread::thread_group;

	queue<int> q{};
	thread_group producers{};
	thread_group consumers{};

	std::atomic_int producer_count{0};
	std::atomic_int consumer_count{0};

	REQUIRE(q.is_empty_unsafe());

	for (auto i = 0ul; i < N_PRODUCERS; ++i)
	{
		producers.emplace_thread(
			producer, 
			std::ref(q), 
			std::ref(producer_count));
	}

	for (auto i = 0ul; i < N_CONSUMERS; ++i)
	{
		consumers.emplace_thread(
			consumer, 
			std::ref(q), 
			std::ref(consumer_count));
	}

	producers.join_all();
	consumers.join_all();

	REQUIRE(producer_count.load() == N_PRODUCERS*WORK_PER_THREAD);
	REQUIRE(consumer_count.load() == N_CONSUMERS*WORK_PER_THREAD);
	REQUIRE(q.is_empty_unsafe());
}