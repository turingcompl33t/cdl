# example/CMakeLists.txt

find_package(Threads REQUIRED)

add_executable(spin_latch "sync/spin_latch.cpp")
target_link_libraries(spin_latch cdl ${CMAKE_THREAD_LIBS_INIT})