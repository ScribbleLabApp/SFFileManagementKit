##===----------------------------------------------------------------------===##
##
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

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Collect all source files for libc
file(GLOB_RECURSE LIBC_SOURCES
    "libc/**/*.c"
    "libc/*.c"

    "../SFFileCoreASM/*.c"
)

# Collect all header files for libc
file(GLOB_RECURSE LIBC_HEADERS
    "libc/**/*.h"
    "libc/*.h"
    "include/libc/*.h"
    "include/libcxx/*.hpp"

    "../SFFIleCoreASM/include/*.h"
)

# Collect all source files for libcxx
file(GLOB_RECURSE LIBCXX_SOURCES
    "libcxx/**/*.cpp"
    "libcxx/**/*.hpp"
    "libcxx/*.cpp"
    "libcxx/*.hpp"
    "include/libcxx/*.hpp"

    "../_SFUtils/*.cpp"
    "../_SFUtils/include/SFCJSON.h"
)

# Create a static library for libc
add_library(SFFileCoreLibc STATIC ${LIBC_SOURCES} ${LIBC_HEADERS})

# Create a static library for libcxx
add_library(SFFileCoreLibcxx STATIC ${LIBCXX_SOURCES})

# Specify include directories for both libraries
target_include_directories(SFFileCoreLibc PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/Sources/SFFileCore/include

    ${CMAKE_CURRENT_SOURCE_DIR}/include/libc
    ${CMAKE_SOURCE_DIR}/Sources/SFFileCore/include/libc
    ../_SFUtils/include
)

target_include_directories(SFFileCoreLibcxx PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/Sources/SFFileCore/include
    
    ${CMAKE_CURRENT_SOURCE_DIR}/include/libcxx
    ${CMAKE_SOURCE_DIR}/Sources/SFFileCore/include/libcxx
    ../_SFUtils/include
)

# Optionally link libcxx with libc
target_link_libraries(SFFileCoreLibcxx PRIVATE SFFileCoreLibc)

# Optionally set properties for the libraries
set_target_properties(SFFileCoreLibc PROPERTIES VERSION 1.0 SOVERSION 1)
set_target_properties(SFFileCoreLibcxx PROPERTIES VERSION 1.0 SOVERSION 1)

if(CLANG_TIDY)
    set_target_properties(SFFileCoreLibc PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY}")
    set_target_properties(SFFileCoreLibcxx PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY}")
endif()
