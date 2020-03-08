# test/CMakeLists.txt

# pull in catch_discover_tests()
include(${CMAKE_SOURCE_DIR}/deps/catch2/contrib/Catch.cmake)

add_library(catch_main STATIC catch_main.cpp)
target_link_libraries(catch_main PUBLIC Catch2::Catch2)

set(FUNCTIONAL_SRCS "functional/with.cpp")
set(ALGORITHM_SRCS "algorithm/invoke_n.cpp")

add_executable(cdl_tests
    ${ALGORITHM_SRCS}
    ${FUNCTIONAL_SRCS})
target_link_libraries(cdl_tests PRIVATE catch_main cdl)

catch_discover_tests(cdl_tests)