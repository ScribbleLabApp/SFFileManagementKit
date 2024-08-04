##===----------------------------------------------------------------------===##
## This source file is part of the ScribbleFoundation open source project
##
## Copyright (c) 2024 ScribbleLabApp. and the Scribble Foundation project authors
## Licensed under Apache License v2.0
##
## See LICENSE for license information
##
## SPDX-License-Identifier: Apache-2.0
##
##===----------------------------------------------------------------------===##

cmake_minimum_required(VERSION 3.10)
project(ScribbleBenchmarks LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE BENCH_SOURCES "*.c")
file(GLOB_RECURSE BENCH_HEADERS "include/*.h")

add_executable(ScribbleBenchmarks main.c ${BENCH_SOURCES})
target_include_directories(ScribbleBenchmarks PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

add_custom_target(scribble_benchmarks
        COMMAND ScribbleBenchmarks
        DEPENDS ScribbleBenchmarks
        COMMENT "Running benchmark suites..."
)

# Enable testing and add a test if needed (Uncomment if you have a test to add)
# enable_testing()
# add_test(NAME BenchmarkTest COMMAND ScribbleBenchmarks)

# Optionally add an install target if needed
# install(TARGETS ScribbleBenchmarks DESTINATION bin)