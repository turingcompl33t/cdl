// invoke_n.cpp
// Unit Test: 
//  - cdl::algorithm::invoke_n()

#include <catch2/catch.hpp>
#include <cdl/algorithm/invoke_n.hpp>

#include <iostream>

constexpr auto N_ITERS = 10;

TEST_CASE("cdl::algorithm_invoke_n() executes the given function the specifed number of times")
{
    using cdl::algorithm::invoke_n;

    auto count = 0u;

    invoke_n(N_ITERS, [&c = count]()
    {
        ++c;
    });

    REQUIRE(count == 10);
}