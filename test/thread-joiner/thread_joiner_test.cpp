// thread_joiner_test.cpp
// Unit test for cdl::thread_joiner.
//
// Build
//	cl /EHsc /nologo /std:c++17 /W4 thread_joiner_test.cpp

#include <vector>
#include <iostream>

#include "../../cdl/thread_joiner.hpp"

constexpr auto N_THREADS = 10;

void thread_fn()
{
	std::cout << "Hello from " << std::this_thread::get_id() << '\n';
}

int main()
{
	std::vector<std::thread> threads;
	for (unsigned i = 0; i < N_THREADS; ++i)
	{
		threads.push_back(
			std::thread{ thread_fn }
		);
	}

	cdl::thread_joiner joiner{ threads };
}