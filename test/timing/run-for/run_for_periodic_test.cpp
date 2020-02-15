// run_for_periodic_test.cpp
// Unit test for cdl::timing::run_for_periodic().
//
// Build
//	cl /EHsc /nologo /std:c++17 /W4 /I C:\Dev\CDL run_for_periodic_test.cpp

#include "cdl/timing/run_for.hpp"

#include <chrono>
#include <iostream>

int main()
{
	using namespace std::chrono_literals;

	cdl::timing::run_for_periodic(
		5s,
		1s,
		[]()
		{
			std::cout << "Task: Execute\n";
		}
	);
}