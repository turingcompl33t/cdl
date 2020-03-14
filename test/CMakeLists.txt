# test/CMakeLists.txt

# pull in catch_discover_tests()
include(${CMAKE_SOURCE_DIR}/deps/catch2/contrib/Catch.cmake)

add_library(catch_main STATIC catch_main.cpp)
target_link_libraries(catch_main PUBLIC Catch2::Catch2)

set(ALGORITHM_SRCS 
    "algorithm/invoke_n.cpp"
    "algorithm/foreach.cpp")
set(CONCURRENT_SRCS 
    "concurrent/queue.cpp"
    "concurrent/stack.cpp")
set(FUNCTIONAL_SRCS 
    "functional/with.cpp")
set(MEMORY_SRCS
    "memory/unique_ptr.cpp")
set(STRING_SRCS 
    "string/basic_string.cpp")
set(THREAD_SRCS 
    "thread/thread_group.cpp")
set(UTILITY_SRCS
    "utility/bit_cast.cpp")

add_executable(cdl_tests
    ${ALGORITHM_SRCS}
    ${CONCURRENT_SRCS}
    ${FUNCTIONAL_SRCS}
    ${MEMORY_SRCS}
    ${STRING_SRCS}
    ${THREAD_SRCS}
    ${UTILITY_SRCS})
    
target_link_libraries(cdl_tests PRIVATE catch_main cdl)

catch_discover_tests(cdl_tests)