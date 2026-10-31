
cmake_minimum_required(VERSION 3.16)
project(imu_tilt_kalman LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)


add_executable(smoke_test tests/smoke_test.cpp)

target_compile_options(smoke_test PRIVATE -Wall -Wextra -Wpedantic)

