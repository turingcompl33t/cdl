// dev_null.cpp
//
// Unit Test: cdl::iterator::dev_null_iterator

#include <catch2/catch.hpp>
#include <cdl/iterator/dev_null.hpp>

#include <vector>
#include <algorithm>

TEST_CASE("cdl::iterator::dev_null_iterator does nothing at all")
{
    using namespace cdl::iterator;

    auto vec = std::vector<int>{1, 2, 3, 4, 5};

    std::copy(
        std::begin(vec),
        std::end(vec),
        dev_null_iterator()
        );

    REQUIRE(true);
}