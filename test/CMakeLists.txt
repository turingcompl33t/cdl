# test/CMakeLists.txt

# pull in catch_discover_tests()
include(${CMAKE_SOURCE_DIR}/deps/catch2/contrib/Catch.cmake)

add_library(catch_main STATIC catch_main.cpp)
target_link_libraries(catch_main PUBLIC Catch2::Catch2)

set(functional_sources "functional/with.cpp")
add_executable(functional_tests ${functional_sources})
target_link_libraries(functional_tests PRIVATE catch_main cdl)
catch_discover_tests(functional_tests)