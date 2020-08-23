# CMakeLists.txt

cmake_minimum_required(VERSION 3.17)

project(cdl CXX)

option(CDL_BUILD_TESTS "Build internal test suite" OFF)
option(CDL_BUILD_EXAMPLES "Build example programs" OFF)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(
    ${PROJECT_NAME} 
    INTERFACE 
    $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)

if(CDL_BUILD_TESTS)
    enable_testing()
    message("Configuring tests...")
    add_subdirectory(test)
    add_subdirectory(deps/catch2)
endif()

if(CDL_BUILD_EXAMPLES)
    message("Configuring examples...")
    add_subdirectory(example)
endif()
