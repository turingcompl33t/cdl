// bit_cast.cpp
// Unit Test: cdl::utility::bit_cast

#include <catch2/catch.hpp>
#include <cdl/utility/bit_cast.hpp>

#include <limits>

TEST_CASE("cdl::utility::bit_cast() allows non-UB casting between")
{
    using cdl::utility::bit_cast;

    // make sure the compiler is using IEEE 754 under the hood
    REQUIRE(std::numeric_limits<float>::is_iec559);

    SECTION("float and int")
    {
        auto f = 1337.0f;
        auto i = bit_cast<int>(f);

        REQUIRE(i == int{0x44a72000});
    }
}