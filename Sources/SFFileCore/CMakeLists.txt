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

# Find OpenSSL package
find_package(OpenSSL REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBXML2 REQUIRED libxml-2.0)

if (OpenSSL_FOUND)
    message(STATUS "Found OpenSSL: ${OPENSSL_VERSION}")
    include_directories(${OPENSSL_INCLUDE_DIR})
else()
    message(FATAL_ERROR "OpenSSL not found")
endif()

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
    "../SFFileCoreBridge/include/*.h"
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
    "../SFFileCoreBridge/include/*.h"
)

# Create a static library for libc
add_library(SFFileCoreLibc STATIC ${LIBC_SOURCES} ${LIBC_HEADERS})

# Create a static library for libcxx
add_library(SFFileCoreLibcxx STATIC ${LIBCXX_SOURCES})

# Include directories
include_directories(${LIBXML2_INCLUDE_DIRS})

# Link directories
link_directories(${LIBXML2_LIBRARY_DIRS})

# Specify include directories for both libraries
target_include_directories(SFFileCoreLibc PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/Sources/SFFileCore/include

    ${CMAKE_CURRENT_SOURCE_DIR}/include/libc
    ${CMAKE_SOURCE_DIR}/Sources/SFFileCore/include/libc
    ../_SFUtils/include
    ../SFFileCoreBridge/include/*.h
)

target_include_directories(SFFileCoreLibcxx PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/Sources/SFFileCore/include
    
    ${CMAKE_CURRENT_SOURCE_DIR}/include/libcxx
    ${CMAKE_SOURCE_DIR}/Sources/SFFileCore/include/libcxx
    ../_SFUtils/include
    ../SFFileCoreBridge/include/*.h
)

# Optionally link libcxx with libc
target_link_libraries(SFFileCoreLibcxx PRIVATE SFFileCoreLibc)

target_link_libraries(SFFileCoreLibc PRIVATE ${LIBXML2_LIBRARIES})
target_link_libraries(SFFileCoreLibc PRIVATE ${OPENSSL_LIBRARIES})
target_link_libraries(SFFileCoreLibcxx PRIVATE ${OPENSSL_LIBRARIES})

# Optionally set properties for the libraries
set_target_properties(SFFileCoreLibc PROPERTIES VERSION 1.0 SOVERSION 1)
set_target_properties(SFFileCoreLibcxx PROPERTIES VERSION 1.0 SOVERSION 1)

if(CLANG_TIDY)
    set_target_properties(SFFileCoreLibc PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY}")
    set_target_properties(SFFileCoreLibcxx PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY}")
endif()
