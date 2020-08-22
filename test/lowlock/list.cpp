// test/lowlock/list.cpp

#include <catch2/catch.hpp>

#include <cdl/lowlock/list.hpp>

struct Dummy
{
    cdl::lowlock::list_entry entry;

    int a;
    int b;

    static auto entry_of(Dummy* d) -> cdl::lowlock::list_entry*
    {
        return &d->entry;
    }

    static auto element_of(cdl::lowlock::list_entry* e) -> Dummy*
    {
        return reinterpret_cast<Dummy*>(
            reinterpret_cast<std::ptrdiff_t>(e) - offsetof(Dummy, entry));
    }
};

TEST_CASE("cdl::lowlock::list supports single-threaded operations")
{
    using cdl::lowlock::list;

    list l{};

    auto d1 = std::make_unique<Dummy>();
    auto d2 = std::make_unique<Dummy>();

    d1->a = 1;
    d1->b = 2;

    d2->a = 3;
    d2->b = 4;

    list::push_back(l, d1->entry);
    list::push_back(l, d2->entry);

    auto d1_opt = list::pop_front(l);
    auto d2_opt = list::pop_front(l);

    REQUIRE(d1_opt.has_value());
    REQUIRE(d2_opt.has_value());

    auto* d1_out = Dummy::element_of(d1_opt.value());
    auto* d2_out = Dummy::element_of(d2_opt.value());

    REQUIRE(d1_out->a == 1);
    REQUIRE(d1_out->b == 2);

    REQUIRE(d2_out->a == 3);
    REQUIRE(d2_out->b == 4);
}

TEST_CASE("cdl::lowlock::list supports unconditional iteration")
{
    using cdl::lowlock::list;
    using cdl::lowlock::list_entry;

    list l{};

    auto d1 = std::make_unique<Dummy>();
    auto d2 = std::make_unique<Dummy>();
    auto d3 = std::make_unique<Dummy>();

    list::push_front(l, d1->entry);
    list::push_front(l, d2->entry);
    list::push_front(l, d3->entry);

    unsigned long n{};

    l.iterate([&n](list_entry* entry){ ++n; });

    REQUIRE(n == 3);
}

TEST_CASE("cdl::lowlock::list supports conditional iteration")
{
    using cdl::lowlock::list;
    using cdl::lowlock::list_entry;

    list l{};

    auto d1 = std::make_unique<Dummy>();
    auto d2 = std::make_unique<Dummy>();
    auto d3 = std::make_unique<Dummy>();

    d3->a = 1;
    d3->b = 1;

    list::push_back(l, d1->entry);
    list::push_back(l, d2->entry);
    list::push_back(l, d3->entry);

    unsigned long n{};

    auto const broke_early = l.iterate_while(
        [&n](list_entry* e){ ++n; }, 
        [](list_entry* e)
        { 
            auto* d = Dummy::element_of(e);
            return (d->a == 1) && (d->b == 1);
        });

    REQUIRE(broke_early);
    REQUIRE(n == 2);
}