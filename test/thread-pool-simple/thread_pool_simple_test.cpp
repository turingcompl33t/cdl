// thread_pool_simple_test.cpp
// Unit test for cdl::thread_pool_simple.
//
// Build
//	cl /EHsc /nologo /std:c++17 /W4 thread_pool_simple_test.cpp

#include <chrono>
#include <iostream>

#include "../../cdl/monitor.hpp"
#include "../../cdl/thread_pool_simple.hpp"

constexpr auto N_WORK_ITEMS = 10;

int main()
{
	using namespace std::chrono_literals;

	cdl::monitor<std::ostream&> sync_cout{ std::cout };

	cdl::thread_pool_simple pool{};

	for (unsigned i = 0; i < N_WORK_ITEMS; ++i)
	{
		pool.submit([=, &sync_cout]()
			{
				sync_cout([=](std::ostream& cout) {cout << "Executing Task " << i << '\n'; });
			}
		);
	}

	pool.wait();
}