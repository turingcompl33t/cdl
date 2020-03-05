// invoke_n_test.cpp
// Unit Test: cdl::algorithm::invoke_n
//
// Build
//	cl /EHsc /nologo /std:c++17 /W4 /I C:\Dev\CDL invoke_n_test.cpp

#include <iostream>

#include "cdl/algorithm/invoke_n.hpp"

constexpr auto N_ITERS = 10;

int main()
{
    unsigned count = 0;

    cdl::algorithm::invoke_n(N_ITERS, [&c = count]()
    {
        c++;
    });

    std::cout << "Count = " << count << '\n';
}