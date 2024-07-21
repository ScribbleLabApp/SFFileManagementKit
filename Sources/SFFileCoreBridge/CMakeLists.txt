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

# Collect all Swift source files
file(GLOB_RECURSE BRIDGE_SOURCES "*.swift")

# Collect all Objective-C++ source and header files
file(GLOB_RECURSE OBJCxx_SOURCES "*.mm")
file(GLOB_RECURSE OBJCxx_HEADERS "*.h")

# Add the library target
add_library(SFFileCoreBridge ${BRIDGE_SOURCES} ${OBJCxx_SOURCES} ${OBJCxx_HEADERS})

# Specify the include directories
target_include_directories(SFFileCoreBridge PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Optionally set properties for the library
set_target_properties(SFFileCoreBridge PROPERTIES VERSION 1.0 SOVERSION 1)
