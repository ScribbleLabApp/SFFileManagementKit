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

file(GLOB_RECURSE ASM_SOURCES
    "*.S"
)

file(GLOB_RECURSE C_SOURCES
    "*.c"
)

file(GLOB_RECURSE C_HEADERS
    "*.h"
)

# Create a static library for the assembly and C code
add_library(SFFileCoreASM STATIC ${ASM_SOURCES} ${C_SOURCES})

# Specify the include directories for the SFFileCoreASM target
target_include_directories(SFFileCoreASM PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Optionally, link against necessary libraries
# target_link_libraries(SFFileCoreASM PUBLIC SomeLibrary)

# Optionally set properties for the library
set_target_properties(SFFileCoreASM PROPERTIES VERSION 1.0 SOVERSION 1)
