// guarded.cpp

#include <catch2/catch.hpp>

#include <cdl/synchronization/critical_section.hpp>
#include <cdl/synchronization/guarded.hpp>

// dummy object to synchronize
struct data
{
    uint8_t x;
    uint8_t y;

    data(uint8_t const x_, uint8_t const y_)
        : x{x_}, y{y_} {}
};

TEST_CASE("cdl::sync::guarded::lock() returns a synchronized, mutable reference")
{
    using cdl::synchronization::guarded;
    using cdl::synchronization::critical_section;

    // construct the guarded object
    // data{1, 2}
    auto g = guarded<data, critical_section>{1, 2};

    {
        // acquire a synchronized reference and mutate
        auto d = g.lock();
        d->x = 19;
        d->y = 20;
    }  // lock released here

    auto d = g.lock();
    REQUIRE(d->x == 19);
    REQUIRE(d->y == 20);
}