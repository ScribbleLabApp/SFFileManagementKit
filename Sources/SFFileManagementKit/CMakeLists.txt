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

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add sources for the SFFileManagementKit module
file(GLOB_RECURSE SOURCES "*.swift")

# Add the Swift library target
add_library(SFFileManagementKit ${SOURCES})

# Specify the include directories
target_include_directories(SFFileManagementKit PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Optionally set properties for the library
set_target_properties(SFFileManagementKit PROPERTIES VERSION 1.0 SOVERSION 1)
