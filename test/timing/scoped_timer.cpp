// scoped_timer_test.cpp
// Unit test for cdl::timing::scoped_timer.
//
// Build
//	 cl /EHsc /nologo /std:c++17 /W4 /I C:\Dev\CDL scoped_timer_test.cpp

#include <thread>
#include <chrono>
#include <iostream>

#include "cdl/timing/scoped_timer.hpp"

int main()
{
	using namespace std::chrono_literals;

	{
		cdl::timing::ms_scoped_timer t{ std::cout };

		std::this_thread::sleep_for(3s);
	}
}