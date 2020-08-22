// test/lowlock/queue.cpp

#include <catch2/catch.hpp>

#include <memory>

#include <cdl/lowlock/queue.hpp>


struct Dummy
{
    int a;
    int b;
    Dummy() 
        : a{}, b{} {}
};

struct MoveOnly
{
    int a;
    int b;

    MoveOnly() : a{}, b{} {}

    MoveOnly(MoveOnly const&)            = delete;
    MoveOnly& operator=(MoveOnly const&) = delete;

    MoveOnly(MoveOnly&& other) 
        : a{other.a}, b{other.b}
    {
        other.a = 0;
        other.b = 0;
    }

    MoveOnly& operator=(MoveOnly&& rhs)
    {
        if (&rhs != this)
        {
            a = rhs.a;
            b = rhs.b;
            rhs.a = 0;
            rhs.b = 0;
        }

        return *this;
    }
};

TEST_CASE("cdl::concurrent::queue supports basic types")
{
    using cdl::lowlock::queue;

    queue<Dummy> q{};

    auto d1 = std::make_unique<Dummy>();
    auto d2 = std::make_unique<Dummy>();

    d1->a = 1;
    d1->b = 1;

    d2->a = 2;
    d2->b = 2;

    q.push(d1.release());
    q.push(d2.release());

    auto const r1 = q.try_pop();
    auto const r2 = q.try_pop();
    auto const r3 = q.try_pop();

    REQUIRE(r1.has_value());
    REQUIRE(r2.has_value());
    REQUIRE_FALSE(r3.has_value());

    auto& d1_out = r1.value();
    auto& d2_out = r2.value();

    REQUIRE(d1_out->a == 1);
    REQUIRE(d1_out->b == 1);

    REQUIRE(d2_out->a == 2);
    REQUIRE(d2_out->b == 2);
}

TEST_CASE("cdl::concurrent::queue supports move-only types")
{
    using cdl::lowlock::queue;

    queue<MoveOnly> q{};

    auto mo1 = std::make_unique<MoveOnly>();
    auto mo2 = std::make_unique<MoveOnly>();

    mo1->a = 1;
    mo1->b = 1;

    mo2->a = 2;
    mo2->b = 2;

    q.push(mo1.release());
    q.push(mo2.release());

    auto const r1 = q.try_pop();
    auto const r2 = q.try_pop();
    auto const r3 = q.try_pop();

    REQUIRE(r1.has_value());
    REQUIRE(r2.has_value());
    REQUIRE_FALSE(r3.has_value());

    auto& mo1_out = r1.value();
    auto& mo2_out = r2.value();

    REQUIRE(mo1_out->a == 1);
    REQUIRE(mo1_out->b == 1);

    REQUIRE(mo2_out->a == 2);
    REQUIRE(mo2_out->b == 2);
}

TEST_CASE("cdl::lowlock::queue supports conditional pop operations")
{
    using cdl::lowlock::queue;

    queue<Dummy> q{};

    auto d1 = std::make_unique<Dummy>();
    auto d2 = std::make_unique<Dummy>();

    d1->a = 1;
    d1->b = 1;

    d2->a = 2;
    d2->b = 2;

    q.push(d1.release());
    q.push(d2.release());

    // attempt to pop an element if sum of members is 4
    auto const r1 = q.try_pop_if([](Dummy* d){ return d->a + d->b == 4; });
    
    REQUIRE_FALSE(r1.has_value());
    
    // attempt to pop an element if sum of members is 2
    auto const r2 = q.try_pop_if([](Dummy* d){ return d->a + d->b == 2; });

    REQUIRE(r2.has_value());

    auto& d1_out = r2.value();
    REQUIRE(d1_out->a == 1);
    REQUIRE(d1_out->b == 1);
}