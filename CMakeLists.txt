
cmake_minimum_required(VERSION 3.16)
project(imu_tilt_kalman LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(googletest
GIT_REPOSITORY https://github.com/google/googletest.git
GIT_TAG v1.18.0
GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(googletest)


add_executable(smoke_test tests/smoke_test.cpp)

target_compile_options(smoke_test PRIVATE -Wall -Wextra -Wpedantic)

target_link_libraries(smoke_test PRIVATE GTest::gtest_main)


