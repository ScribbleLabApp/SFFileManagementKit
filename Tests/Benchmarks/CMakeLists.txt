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

## Find dependencies
find_package(OpenSSL REQUIRED)
if (OpenSSL_FOUND)
    message(STATUS "Found OpenSSL: ${OPENSSL_VERSION}")
    include_directories(${OPENSSL_INCLUDE_DIR})
else()
    message(FATAL_ERROR "OpenSSL not found")
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBXML2 REQUIRED libxml-2.0)

## Include needed target directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../Sources/SFFileCore/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../Sources/_SFUtils/include)

## Include and Link libxml-2.0 headers
include_directories(${LIBXML2_INCLUDE_DIRS})
link_directories(${LIBXML2_LIBRARY_DIRS})

add_subdirectory(${SFUTILS_SCR_DIR} ${SFUTILS_BIN_DIR})

## Scribble Benchmark sources/headers
file(GLOB_RECURSE BENCH_SOURCES "*.c")
file(GLOB_RECURSE BENCH_HEADERS "include/*.h")

## Add executable main.c
add_executable(ScribbleBenchmarks main.c ${BENCH_SOURCES})

## ScribbleBenchmark include directory
target_include_directories(ScribbleBenchmarks PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

## Link Dependencies
target_link_libraries(ScribbleBenchmarks PRIVATE
    ${OPENSSL_LIBRARIES}
    ${LIBXML2_LIBRARIES}
    #../../build/lib/libSFUtils.a
)

## Optimization flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

## Run benchmark executable
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