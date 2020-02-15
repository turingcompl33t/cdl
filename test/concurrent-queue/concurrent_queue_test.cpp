// concurrent_queue_test.cpp
// Unit test for cdl::concurrent_queue.
//
// Build
//	cl /EHsc /nologo /std:c++17 /W4 concurrent_queue_test.cpp

#include <thread>
#include <vector>
#include <iostream>

#include "../../cdl/monitor.hpp"
#include "../../cdl/thread_joiner.hpp"
#include "../../cdl/concurrent_queue.hpp"

constexpr auto N_PRODUCERS = 10;
constexpr auto N_CONSUMERS = 10;

cdl::concurrent_queue<int> work_queue{};
cdl::monitor<std::ostream&> sync_cout{ std::cout };

void producer_fn(unsigned index)
{
	work_queue.push(index);
	sync_cout([=](std::ostream& cout) {cout << "Produced: " << index << '\n'; });
}

void consumer_fn()
{
	auto item = work_queue.wait_and_pop();
	sync_cout([=](std::ostream& cout) {cout << "Consumed: " << *item << '\n'; });
}

int main()
{
	std::vector<std::thread> threads{};

	for (unsigned i = 0; i < N_PRODUCERS; ++i)
	{
		threads.push_back(
			std::thread{ producer_fn, i }
		);
	}

	for (unsigned i = 0; i < N_CONSUMERS; ++i)
	{
		threads.push_back(
			std::thread{ consumer_fn }
		);
	}

	// ensure all threads joined
	cdl::thread_joiner joiner{ threads };
}