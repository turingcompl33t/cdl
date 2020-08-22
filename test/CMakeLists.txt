# test/CMakeLists.txt

# pull in catch_discover_tests()
include(${CMAKE_SOURCE_DIR}/deps/catch2/contrib/Catch.cmake)

find_package(Threads REQUIRED)

add_library(catch_main STATIC catch_main.cpp)
target_link_libraries(catch_main PUBLIC Catch2::Catch2)

SET(CONCURRENT_SRCS
    "concurrent/queue.cpp"
    "concurrent/stack.cpp")

SET(LOWLOCK_SRCS
    "lowlock/list.cpp"
    "lowlock/queue.cpp"
    "lowlock/spinlock.cpp")

SET(SYNC_SRCS
    "sync/latch.cpp"
    "sync/spin_latch.cpp")

SET(UTILITY_SRCS
    "utility/voidify.cpp")

add_executable(cdl_tests 
    ${CONCURRENT_SRCS}
    ${LOWLOCK_SRCS}
    ${SYNC_SRCS}
    ${UTILITY_SRCS})
    
target_link_libraries(cdl_tests PRIVATE catch_main cdl ${CMAKE_THREAD_LIBS_INIT})

catch_discover_tests(cdl_tests)