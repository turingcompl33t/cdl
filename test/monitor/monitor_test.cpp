// monitor_test.cpp
// Unit test for cdl::monitor.
//
// Build
//	cl /EHsc /nologo /std:c++17 /W4 monitor_test.cpp

#include <thread>
#include <future>
#include <iostream>

#include "../../cdl/monitor.hpp"

constexpr auto N_TASKS = 5;

int main()
{
	std::vector<std::future<void>> futures{};

	cdl::monitor<std::ostream&> sync_cout{ std::cout };

	sync_cout([](std::ostream& cout) { cout << "START\n"; });

	for (unsigned i = 0; i < N_TASKS; ++i)
	{
		futures.push_back(std::async(std::launch::async, [&, i]() {
			
			sync_cout([=](std::ostream& cout) {
				cout << "Hello #1 from future " << i << '\n';
			});

			sync_cout([=](std::ostream& cout) {
				cout << "Hello #2 from future " << i << '\n';
			});
		
		}));
	}

	for (auto& f : futures)
	{
		f.wait();
	}

	sync_cout([](std::ostream& cout) {cout << "FINISH\n"; });
}