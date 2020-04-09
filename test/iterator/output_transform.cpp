// output_transform.cpp
//
// Unit Test: cdl::iterator::output_transform_iterator

#include <catch2/catch.hpp>
#include <cdl/iterator/output_transform.hpp>

#include <vector>
#include <algorithm>

TEST_CASE("cdl::iterator::output_transform_iterator")
{
    using namespace cdl::iterator;

    auto src = std::vector<int>{1, 2, 3, 4, 5};
    auto dst = std::vector<int>{};

    auto const times_two = make_output_transformer([](int value){ return value*2; });

    std::copy(
        std::begin(src), 
        std::end(src), 
        times_two(std::back_inserter(dst))
        );

    auto res = std::equal(
        std::begin(dst), 
        std::end(dst),
        std::vector{2, 4, 6, 8, 10}
        );

    REQUIRE(true);
}