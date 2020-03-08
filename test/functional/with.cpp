// with.cpp
// Unit Test: 
//  - cdl::functional::with()
//  - cdl::functional::operator|

#include <catch2/catch.hpp>
#include <cdl/functional/with.hpp>

#include <optional>
#include <sstream>

TEST_CASE("cdl::functional::with() behaves as expected")
{
    using cdl::functional::with;

    SECTION("when input is present std::optional")
    {
        auto i = std::optional<int>{1};

        REQUIRE(i.has_value());

        auto r = with(i, [](int j){ return j*2; });

        REQUIRE(r.has_value());
        REQUIRE(*r == 2);
    }

    SECTION("when input is absent std::optional")
    {
        auto i = std::optional<int>{};

        REQUIRE_FALSE(i.has_value());

        auto r = with(i, [](int j){ return j* 2; });

        REQUIRE_FALSE(r.has_value());
    }
}

TEST_CASE("cdl::functional::operator| behaves as expected")
{
    using cdl::functional::operator|;

    SECTION("when input is present std::optional")
    {
        auto i = std::optional<int>{1};

        REQUIRE(i.has_value());

        auto r = i | [](int j){ return j*2; };

        REQUIRE(r.has_value());
        REQUIRE(*r == 2);
    }

    SECTION("when input is absent std::optional")
    {
        auto i = std::optional<int>{};

        REQUIRE_FALSE(i.has_value());

        auto r = i | [](int j){ return j*2; };

        REQUIRE_FALSE(r.has_value());
    }
}

TEST_CASE("cdl::functional::with() composes")
{
    using cdl::functional::with;

    SECTION("when value is present throughout composition")
    {
        auto i = std::optional<int>{1};

        REQUIRE(i.has_value());

        auto r = with(
            with(i, [](int j){ return j+1; }),
            [](int k){ return k*2; }
            );
        
        REQUIRE(r.has_value());
        REQUIRE(*r == 4);
    }

    SECTION("when inner expression returns absent std::optional")
    {
        auto i = std::optional<int>{1};

        REQUIRE(i.has_value());

        auto r = with(
            with(i, [](int j){ return std::optional<int>{}; }),
            [](int k){ return k*2; }
            );

        REQUIRE_FALSE(r.has_value());
    }
}

TEST_CASE("cdl::functional::operator| composes")
{
    using cdl::functional::operator|;

    SECTION("when value is present throughout composition")
    {
        auto i = std::optional<int>{1};

        REQUIRE(i.has_value());

        auto r = i
            | [](int j){ return j+1; }
            | [](int k){ return k*2; };

        REQUIRE(r.has_value());
        REQUIRE(*r == 4);
    }

    SECTION("when inner expression returns absent optional")
    {
        auto i = std::optional<int>{1};

        REQUIRE(i.has_value());

        auto r = i
            | [](int j){ return std::optional<int>{}; }
            | [](int k){ return k*2; };

        REQUIRE_FALSE(r.has_value());
    }
}

TEST_CASE("cdl::functional::operator| makes dope compositional pipelines possible")
{
    using cdl::functional::operator|;

    auto i = std::optional<int>{1};

    auto r = i
        | [](int j){ return j*2; }
        | [](int k){ return k < 10 ? k : std::optional<int>{}; }
        | [](int l){ return l+1; }
        | [](int m){ std::ostringstream oss{}; oss << m; return oss.str(); };

    REQUIRE(r.has_value());
    REQUIRE(*r == "3");
}