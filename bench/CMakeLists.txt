# bench/CMakeLists.txt

add_executable(bench-spsc "lockfree/spsc.cpp")
target_link_libraries(bench-spsc cdl benchmark)