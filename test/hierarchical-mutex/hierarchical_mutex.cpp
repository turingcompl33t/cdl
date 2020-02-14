// hierarchical_mutex.cpp
// Unit test for hierarchical mutex basic functionality.

#define CATCH_CONFIG_MAIN
#include "../../deps/catch/catch.hpp"

#include "../../cdl/hierarchical_mutex.hpp"

TEST_CASE("hierarchical_mutex")
{
	cdl::hierarchical_mutex hmutex{ 10000 };
}