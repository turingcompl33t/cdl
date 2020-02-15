// exponential_wait_test.cpp
// Unit test for cdl::timing::exponential_wait.
//
// Build
//	cl /EHsc /nologo /std:c++17 /W4 /I C:\Dev\CDL exponential_wait_test.cpp

#include <thread>
#include <chrono>
#include <iostream>

#include "cdl/timing/exponential_wait.hpp"

int main()
{
	cdl::timing::exponential_wait<std::chrono::milliseconds> exp_wait{ 1000. };

	for (;;)
	{
		auto begin = std::chrono::steady_clock::now();
		
		std::this_thread::sleep_for(exp_wait.next());
		
		auto duration = std::chrono::steady_clock::now() - begin;
		auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration);

		std::cout << "Slept: " << millis.count() << " milliseconds\n";
	}
}