# test/CMakeLists.txt

# pull in catch_discover_tests()
include(${CMAKE_SOURCE_DIR}/deps/catch2/contrib/Catch.cmake)

find_package(Threads REQUIRED)

add_library(catch_main STATIC catch_main.cpp)
target_link_libraries(catch_main PUBLIC Catch2::Catch2)

SET(SYNC_SRCS
    "sync/latch.cpp"
    "sync/spin_latch.cpp")

add_executable(cdl_tests ${SYNC_SRCS})
    
target_link_libraries(cdl_tests PRIVATE catch_main cdl ${CMAKE_THREAD_LIBS_INIT})

catch_discover_tests(cdl_tests)