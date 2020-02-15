// stack_test.cpp
// Unit test for cdl::concurrent::stack.
//
// Build
//	cl /EHsc /nologo /std:c++17 /W4 /I C:\Dev\CDL stack_test.cpp

#include <thread>
#include <vector>
#include <iostream>

#include "cdl/concurrent/stack.hpp"
#include "cdl/thread/thread_joiner.hpp"
#include "cdl/synchronization/monitor.hpp"

constexpr auto N_PRODUCERS = 10;
constexpr auto N_CONSUMERS = 10;

cdl::concurrent::stack<int> work_stack{};
cdl::synchronization::monitor<std::ostream&> sync_cout{ std::cout };

void producer_fn(unsigned index)
{
	work_stack.push(index);
	sync_cout([=](std::ostream& cout) {cout << "Pushed: " << index << '\n'; });
}

void consumer_fn()
{
	std::shared_ptr<int> item;

	try
	{
		item = work_stack.pop();
	}
	catch (cdl::concurrent::empty_stack_exception&)
	{
		sync_cout([=](std::ostream& cout) {cout << "Empty Stack Exception\n"; });
		return;
	}

	sync_cout([=](std::ostream& cout) {cout << "Popped: " << *item << '\n'; });
}

int main()
{
	std::vector<std::thread> threads{};
	cdl::thread::thread_joiner joiner{ threads };

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

	// join on all threads
	joiner.wait_all();

	while (!work_stack.empty())
	{
		auto item = work_stack.pop();
		std::cout << "Drained: " << *item << '\n';
	}
}