// thread_group.cpp
// Unit Test: cdl::thread::thread_group

#include <catch2/catch.hpp>
#include <cdl/thread/thread_group.hpp>

#include <thread>

TEST_CASE("cdl::thread::thread_group supports adding existing threads")
{
    using cdl::thread::thread_group;

    thread_group group{};

    auto thread_ran = false;

    auto t = std::thread{[&b = thread_ran](){ b = true; }};

    group.add_thread(std::move(t));

    group.join_all();

    REQUIRE(thread_ran);
    REQUIRE(group.count() == 1);
}

TEST_CASE("cdl::thread::thread_group supports emplacing new threads")
{
    using cdl::thread::thread_group;

    thread_group group{};

    auto thread_ran = false;

    group.emplace_thread([&b = thread_ran](){ b = true; });

    group.join_all();

    REQUIRE(thread_ran);
    REQUIRE(group.count() == 1);
}