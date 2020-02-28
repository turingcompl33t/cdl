// stack_test.cpp
// Unit test for cdl::lockfree::stack.
//
// Build
//	cl /EHsc /nologo /std:c++17 /W4 /I C:\Dev\CDL stack_test.cpp

#include <thread>
#include <vector>
#include <iostream>

#include "cdl/lockfree/stack.hpp"
#include "cdl/thread/thread_joiner.hpp"
#include "cdl/synchronization/monitor.hpp"

constexpr auto N_PRODUCERS = 10;
constexpr auto N_CONSUMERS = 10;

cdl::lockfree::stack<int> work_stack{};
cdl::synchronization::monitor<std::ostream&> sync_cout{ std::cout };

void producer_fn(int index)
{
	work_stack.push(index);
	sync_cout([=](std::ostream& cout) {cout << "Pushed: " << index << '\n'; });
}

void consumer_fn()
{
	auto item = work_stack.pop();
	sync_cout([=](std::ostream& cout) {cout << "Popped: " << *item << '\n'; });
}

int main()
{
	std::vector<std::thread> threads{};
	cdl::thread::thread_joiner joiner{ threads };

	for (int i = 0; i < N_PRODUCERS; ++i)
	{
		threads.push_back(
			std::thread{ producer_fn, i }
			);
	}

	for (int i = 0; i < N_CONSUMERS; ++i)
	{
		threads.push_back(
			std::thread{ consumer_fn }
			);
	}

	// join on all threads
	joiner.wait_all();
}