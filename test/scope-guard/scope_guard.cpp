// scope_guard.cpp
// Unit test for scope guard and declarative control flow macros.

#define CATCH_CONFIG_MAIN
#include "../../deps/catch/catch.hpp"

#include <iostream>
#include "../../cdl/scope_guard.h"

TEST_CASE("NullTest")
{
	REQUIRE(true);
}