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

# Define the minimum CMake version and the project
cmake_minimum_required(VERSION 3.15.1)
project(SFFileManagementKit LANGUAGES C CXX ASM Swift)

# Set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_Swift_MODULE_DIRECTORY ${CMAKE_BINARY_DIR}/swift)

# Optionally set flags for building shared libraries
option(BUILD_SHARED_LIBS "Build shared libraries by default" YES)
if(BUILD_SHARED_LIBS)
  set(CMAKE_POSITION_INDEPENDENT_CODE YES)
endif()

# Add subdirectories if they exist
if(EXISTS "${CMAKE_SOURCE_DIR}/Sources/SFFileCore")
    add_subdirectory(Sources/SFFileCore)
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/Sources/SFFileCoreASM")
    add_subdirectory(Sources/SFFileCoreASM)
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/Sources/SFFileCoreBridge")
    add_subdirectory(Sources/SFFileCoreBridge)
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/Sources/SFFileManagementKit")
    add_subdirectory(Sources/SFFileManagementKit)
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/_SFUtils")
    add_subdirectory(_SFUtils)
endif()

# Comment out or remove this line if the tests directory is not yet available
# if(EXISTS "${CMAKE_SOURCE_DIR}/Tests/SFFileManagementKitTests")
#     add_subdirectory(Tests/SFFileManagementKitTests)
# endif()
