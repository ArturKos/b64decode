cmake_minimum_required(VERSION 3.14)
project(b64decode CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

add_compile_options(-Wall -Wextra -Wpedantic)

add_library(b64decode_lib STATIC src/base64_decoder.cpp)
target_include_directories(b64decode_lib PUBLIC include)

add_executable(b64decode src/main.cpp)
target_link_libraries(b64decode PRIVATE b64decode_lib)

option(BUILD_TESTING "Build the unit test suite" ON)
if(BUILD_TESTING)
    enable_testing()
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.14.0
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    add_executable(b64decode_tests tests/test_base64_decoder.cpp)
    target_link_libraries(b64decode_tests PRIVATE b64decode_lib GTest::gtest_main)

    include(GoogleTest)
    gtest_discover_tests(b64decode_tests)
endif()
