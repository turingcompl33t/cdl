// voidify.cpp
// 
// Unit Test: cdl::utility::voidify()

#include <catch2/catch.hpp>
#include <cdl/utility/voidify.hpp>

using func1_f = void(*)(void*);
using func2_f = void(*)(void*, int);

void register_callback1(func1_f fn, void* p)
{
    fn(p);
}

void register_callback2(func2_f fn, void* p, int n)
{
    fn(p, n);
}

TEST_CASE("cdl::utility::voidify() supports passing capturing lambda to C-style interface")
{
    auto x = int{0};

    auto closure = [&x](){ ++x; };
    auto voidified = cdl::utility::voidify(closure);

    register_callback1(voidified.first, voidified.second.get());

    REQUIRE(x == 1);

    register_callback1(voidified.first, voidified.second.get());

    REQUIRE(x == 2);
}

TEST_CASE("cdl::utility::voidify() supports passing capturing lambda with arguments to C-style interface")
{
    auto x = int{0};

    auto closure = [&x](int n){ x += n; };
    auto voidified = cdl::utility::voidify<int>(closure);

    register_callback2(voidified.first, voidified.second.get(), 4);

    REQUIRE(x == 4);

    register_callback2(voidified.first, voidified.second.get(), 6);

    REQUIRE(x == 10);
}