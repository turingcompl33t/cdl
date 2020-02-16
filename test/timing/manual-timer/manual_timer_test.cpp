// manual_timer_test.cpp
// Unit test for cdl::timing::manual_timer.
//
// Build
//	cl /EHsc /nologo /std:c++17 /W4 /I C:\Dev\CDL manual_timer_test.cpp

#include <thread>
#include <chrono>
#include <iostream>

#include "cdl/timing/manual_timer.hpp"

int main()
{
	using namespace std::chrono_literals;

	cdl::timing::ms_manual_timer t{};

	t.start();

	std::this_thread::sleep_for(3s);

	auto elapsed = t.stop();

	std::cout << elapsed.count() << "ms elapsed\n";
}