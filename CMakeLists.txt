cmake_minimum_required(VERSION 3.10)

project(base64)

set (CMAKE_CXX_STANDARD 11)
#set (CMAKE_CXX_FLAGS "-O3 -fno-vectorize -Rpass-analysis=.* -Rpass=.* -Rpass-missed=.*")
set (CMAKE_CXX_FLAGS "-O3 -fno-vectorize")
set (CMAKE_C_FLAGS "-O3")
find_package(benchmark REQUIRED)

add_executable(bench bench.cpp)
target_link_libraries(bench benchmark::benchmark)

add_executable(check sanity_check.cpp)
