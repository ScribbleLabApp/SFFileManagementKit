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

find_program(NASM_EXECUTABLE nasm)
if (NOT NASM_EXECUTABLE)
    message(FATAL_ERROR "NASM not found. Please install NASM.")
endif()

set(ASM_SOURCES_X86
    crc32_x86.asm
)

set(ASM_SOURCES_ARM64
    crc32_arm64.asm
)

set(C_SOURCES
    crc32.c
)

set(LIBRARY_NAME SFFileCoreASM)

if(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
    if(APPLE)
        set(CMAKE_ASM_NASM_OBJECT_FORMAT macho64)
    else()
        set(CMAKE_ASM_NASM_OBJECT_FORMAT elf64)
    endif()
    add_library(${LIBRARY_NAME} STATIC ${C_SOURCES} ${ASM_SOURCES_X86})

elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
    if(APPLE)
        set(CMAKE_ASM_NASM_OBJECT_FORMAT macho64)
    else()
        set(CMAKE_ASM_NASM_OBJECT_FORMAT elf64)
    endif()
    add_library(${LIBRARY_NAME} STATIC ${C_SOURCES} ${ASM_SOURCES_ARM64})

else()
    message(FATAL_ERROR "Unsupported architecture: ${CMAKE_SYSTEM_PROCESSOR}")
endif()

set(CMAKE_ASM_NASM_COMPILER ${NASM_EXECUTABLE})

target_include_directories(${LIBRARY_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)
