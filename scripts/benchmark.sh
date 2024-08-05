#!/bin/bash

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

if [ "$VERBOSE" = "true" ]; then
    set -x
fi

BUILD_DIR="../benchmark"

GREEN='\033[1;32m'
RED='\033[0;31m'
ORANGE='\033[0;33m'
BLUE='\033[0;34m'
BOLD='\033[1m'
RESET='\033[0m'

if [[ "$TERM" != "xterm-256color" ]] && ! command -v tput &> /dev/null; then
    echo "Warning: ANSI escape codes not supported. Falling back to basic text formatting."
    bold() { echo -e "${BOLD}$1${RESET}"; }
    red() { echo -e "${RED}$1${RESET}"; }
    green() { echo -e "${GREEN}$1${RESET}"; }
    orange() { echo -e "${ORANGE}$1${RESET}"; }
    blue() { echo -e "${BLUE}$1${RESET}"; }
    reset() { echo -e "$1"; }
else
    bold() { tput bold; echo -e "$1"; tput sgr0; }
    red() { tput setaf 1; echo -e "$1"; tput sgr0; }
    green() { tput setaf 2; tput bold; echo -e "$1"; tput sgr0; }
    orange() { tput setaf 3; echo -e "$1"; tput sgr0; }
    blue() { tput setaf 4; echo -e "$1"; tput sgr0; }
    reset() { tput sgr0; echo -e "$1"; }
fi

bold "${BLUE}Starting the benchmark tests for SFFileManagementKit${RESET}"
echo ""
echo "Version: 0.1.0-beta (3)"
echo "Copyright (c) 2024 - ScribbleLabApp. All rights reserved."
echo ""

if [ -d "$BUILD_DIR" ]; then
    bold "${ORANGE}==>${RESET} Benchmark directory exists. Cleaning up..."
    rm -rf "$BUILD_DIR"
fi

if [ -n "$GITHUB_WORKSPACE" ]; then
    SOURCE_DIR="$GITHUB_WORKSPACE"
else
    SOURCE_DIR="../Tests/Benchmarks"
fi

echo ""
bold "${ORANGE}==>${RESET} Checking for Ninja..."
if ! command -v ninja &> /dev/null; then
    bold "${RED}[ERROR]: Ninja not found."
    bold "${RED}         Benchmark script exits with nonzero exit code.${RESET}"
    exit 1;
else
    bold "${BLUE}==>${RESET} Ninja found."
fi

echo ""
bold "${ORANGE}==>${RESET} Creating benchmark directory..."
mkdir "$BUILD_DIR"
cd "$BUILD_DIR" || { red "Failed to change directory to $BUILD_DIR"; exit 1; }

echo ""
bold "${ORANGE}==>${RESET} Configuring the project..."
#cmake -G Ninja "$SOURCE_DIR" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON || { red "CMake configuration failed"; exit 1; }
cmake -G Ninja  \
  -D SFUTILS_BIN_DIR:PATH="build/lib/" \
  -D SFUtils_SRC_DIR:PATH="Sources/_SFUtils" \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
  "$SOURCE_DIR" || { red "CMake configuration failed"; exit 1; }

echo ""
bold "${ORANGE}==>${RESET} Compiling & Building the project..."
ninja || { red "${BOLD}[ERROR]: Build failed${RESET}"; exit 1; }

bold "${GREEN}[SUCCESS]: Build complete.${RESET}"
echo ""

bold "${ORANGE}==>${RESET} Running benchmark suites..."
cmake --build . --target scribble_benchmarks