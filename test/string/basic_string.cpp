// basic_string.cpp
// Unit Test: cdl::string::basic_string

#include <catch2/catch.hpp>
#include <cdl/string/string.hpp>

TEST_CASE("cdl::string::basic_string<char> semantics behave as expected")
{
    using cdl::string::string;

    SECTION("default construction")
    {
        auto s = string{};
        REQUIRE(s.empty());
        REQUIRE(s.length() == 0);
    }

    SECTION("construction from raw string literal")
    {
        auto s = string{"hello"};

        REQUIRE_FALSE(s.empty());
        REQUIRE(s.length() == 5);
    }

    SECTION("copy construction")
    {
        auto src = string{"Hello"};
        auto dst = string{src};

        REQUIRE_FALSE(dst.empty());
        REQUIRE(dst.length() == 5);
        REQUIRE(dst.is_equal(src));
    }

    SECTION("copy assignment")
    {
        auto src = string{"hello"};
        auto dst = src;

        REQUIRE_FALSE(dst.empty());
        REQUIRE(dst.length() == 5);
        REQUIRE(dst.is_equal(src));
    }

    SECTION("move construction")
    {
        auto src = string{"hello"};
        auto dst{std::move(src)};

        REQUIRE_FALSE(dst.empty());
        REQUIRE(dst.length() == 5);
        REQUIRE(dst.is_equal("hello"));
    }

    SECTION("move assignment")
    {
        auto src = string{"hello"};
        auto dst = std::move(src);

        REQUIRE_FALSE(dst.empty());
        REQUIRE(dst.length() == 5);
        REQUIRE(dst.is_equal("hello"));
    }
}

TEST_CASE("cdl::string::basic_string<wchar_t> semantics behave as expected")
{
    using cdl::string::wstring;

    SECTION("default construction")
    {
        auto ws = wstring{};
        REQUIRE(ws.empty());
        REQUIRE(ws.length() == 0);
    }

    SECTION("construction from raw string literal")
    {
        auto ws = wstring{L"hello"};
        
        REQUIRE_FALSE(ws.empty());
        REQUIRE(ws.length() == 5);
    }

    SECTION("copy construction")
    {
        auto src = wstring{L"Hello"};
        auto dst = wstring{src};

        REQUIRE_FALSE(dst.empty());
        REQUIRE(dst.length() == 5);
        REQUIRE(dst.is_equal(src));
    }

    SECTION("copy assignment")
    {
        auto src = wstring{L"hello"};
        auto dst = src;

        REQUIRE_FALSE(dst.empty());
        REQUIRE(dst.length() == 5);
        REQUIRE(dst.is_equal(src));
    }

    SECTION("move construction")
    {
        auto src = wstring{L"hello"};
        auto dst{std::move(src)};

        REQUIRE_FALSE(dst.empty());
        REQUIRE(dst.length() == 5);
        REQUIRE(dst.is_equal(L"hello"));
    }

    SECTION("move assignment")
    {
        auto src = wstring{L"hello"};
        auto dst = std::move(src);

        REQUIRE_FALSE(dst.empty());
        REQUIRE(dst.length() == 5);
        REQUIRE(dst.is_equal(L"hello"));
    }
}

