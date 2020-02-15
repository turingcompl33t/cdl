// run_for_test.cpp
// Unit test for cdl::timing::run_for().
//
// Build
//	cl /EHsc /nologo /std:c++17 /W4 /I C:\Dev\CDL run_for_test.cpp

#include <thread>
#include <chrono>
#include <iostream>

#include "cdl/timing/run_for.hpp"

int main()
{
	using namespace std::chrono_literals;

	cdl::timing::run_for(
		5s,
		[]()
		{
			std::cout << "Task: Enter\n";
			std::this_thread::sleep_for(1s);
			std::cout << "Task: Exit\n";
		}
	);
}