// uniform_wait_test.cpp
// Unit test for cdl::timing::uniform_wait.
//
// Build
//	cl /EHsc /nologo /std:c++17 /W4 /I C:\Dev\CDL uniform_wait_test.cpp

#include <thread>
#include <chrono>
#include <iostream>

#include "cdl/timing/uniform_wait.hpp"

int main()
{
	cdl::timing::uniform_wait<std::chrono::milliseconds, unsigned> uni_wait{ 500, 1500 };

	for (;;)
	{
		auto begin = std::chrono::steady_clock::now();

		std::this_thread::sleep_for(uni_wait.next());

		auto duration = std::chrono::steady_clock::now() - begin;
		auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration);

		std::cout << "Slept: " << millis.count() << " milliseconds\n";
	}
}