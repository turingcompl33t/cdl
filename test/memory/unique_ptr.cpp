// unique_ptr.cpp
// Unit Test: cdl::memory::unique_ptr

#include <catch2/catch.hpp>
#include <cdl/memory/unique_ptr.hpp>

struct X
{
    int        m_id;
    static int count;

    X(int id) : m_id{id}
    {
        ++count;
    }

    ~X()
    {
        --count;
    }

    int get_id() const noexcept
    {
        return m_id;
    }
};

int X::count{0};

TEST_CASE("cdl::memory::unique_ptr is an RAII wrapper")
{
    using cdl::memory::unique_ptr;

    REQUIRE(X::count == 0);

    auto p1 = unique_ptr<X>{new X{1337}};

    REQUIRE(X::count == 1);
    REQUIRE(p1);

    {
        auto p2 = unique_ptr<X>{new X{1054}};
        REQUIRE(X::count == 2);
        REQUIRE(p2);
    }

    REQUIRE(X::count == 1);
}

TEST_CASE("cdl::memory::unique_ptr provides the make_unique convenience function")
{
    using cdl::memory::make_unique;

    auto ptr = make_unique<X>(1337);

    REQUIRE(ptr);
    REQUIRE(ptr->get_id() == 1337);
}