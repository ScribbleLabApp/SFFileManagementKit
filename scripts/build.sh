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

# This script configures and builds the project using CMake with the Xcode generator.

# Define build directory relative to the project root
BUILD_DIR="../build"

# Define color codes for terminal output
GREEN='\033[1;32m'
RED='\033[0;31m'
ORANGE='\033[0;33m'
BLUE='\033[0;34m'
BOLD='\033[1m'
RESET='\033[0m'

# Define functions for colored output
if [[ "$TERM" != "xterm-256color" ]] && ! command -v tput &> /dev/null; then
    echo "Warning: ANSI escape codes not supported. Falling back to basic text formatting."
    # Define basic text formatting functions
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

# Print introductory information
bold "${BLUE}Starting the build process for SFFileManagementKit${RESET}"
echo ""
echo "Version: 0.1.0-beta (1)"
echo "Copyright (c) 2024 - ScribbleLabApp. All rights reserved."
echo ""

# Check for existing build directory and remove it if it exists
if [ -d "$BUILD_DIR" ]; then
    bold "${ORANGE}==>${RESET} Build directory exists. Cleaning up..."
    rm -rf "$BUILD_DIR"
fi

if [ -n "$GITHUB_WORKSPACE" ]; then
    SOURCE_DIR="$GITHUB_WORKSPACE"
else
    SOURCE_DIR=".." # Adjust this if your CMakeLists.txt is not in the parent directory locally
fi

echo ""
bold "${ORANGE}==>${RESET} Checking for Ninja..."
if ! command -v ninja &> /dev/null; then
    bold "${BLUE}==>${RESET} Ninja not found."
    bold "${ORANGE}==>${RESET} Installing Ninja..."
    brew install ninja
    
    bold "${ORANGE}==>${RESET} Verify ninja installation..."
    if ! command -v ninja &> /dev/null; then
        bold "${RED}[ERROR]: Ninja not found."
        bold "${RED}         Build script exits with nonzero exit code.${RESET}"
    else
        bold "${BLUE}==>${RESET} Ninja found. Proceeding with build"
    fi
else
    bold "${BLUE}==>${RESET} Ninja found."
fi

# Create the build directory
echo ""
bold "${ORANGE}==>${RESET} Creating build directory..."
mkdir "$BUILD_DIR"
cd "$BUILD_DIR" || { red "Failed to change directory to $BUILD_DIR"; exit 1; }

# Configure the project using CMake with Xcode generator
echo ""
bold "${ORANGE}==>${RESET} Configuring the project..."
#cmake -G Xcode "$SOURCE_DIR" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON || { red "CMake configuration failed"; exit 1; }
cmake -G Ninja "$SOURCE_DIR" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON || { red "CMake configuration failed"; exit 1; }

# Build the project using Xcode
echo ""
bold "${ORANGE}==>${RESET} Building the project..."
#xcodebuild -project SFFileManagementKit.xcodeproj -scheme SFFileManagementKit || { red "Build failed"; exit 1; }
ninja || { red "${BOLD}[ERROR]: Build failed${RESET}"; exit 1; }

bold "${GREEN}[SUCCESS]: Build complete.${RESET}"
echo ""

##===------------------------------------- Linting -------------------------------------===##

# Ensure clang-tidy is available
bold "${ORANGE}==>${RESET} Check for clang-tidy..."
if ! command -v clang-tidy &> /dev/null; then
    echo ""
    bold "${BLUE}==>${RESET} clang-tidy not found."
    bold "${ORANGE}==>${RESET} Installing clang-tidy..."
    if ! command -v brew &> /dev/null; then
        echo ""
        bold "${RED}[ERROR]: Homebrew is not installed. Please install Homebrew first.${RESET}"
        bold "${RED}         ScribbleLabApp Build Script exits with nonzero exit code.${RESET}"
        exit 1
    fi
    brew install llvm
    bold "${BLUE}==>${RESET} clang-tidy installed"
    echo ""
fi

# Ensure correct PATH for clang-tidy
if [[ "$OSTYPE" == "darwin"* ]]; then
    export PATH="/usr/local/opt/llvm/bin:$PATH"        # For Intel Macs
    if [[ $(uname -m) == "arm64" ]]; then
        export PATH="/opt/homebrew/opt/llvm/bin:$PATH" # For Apple Silicon Macs
    fi
fi

# Verify clang-tidy installation
if ! command -v clang-tidy &> /dev/null; then
    red "${BOLD}[ERROR]: clang-tidy not found even after installation.${RESET}"
    red "${BOLD}         ScribbleLabApp Build Script exits with nonzero exit code.${RESET}"
    exit 1
fi

# Check for compile_commands.json
echo ""
bold "${ORANGE}==>${RESET} Verify compilation database..."
if [ ! -f "$BUILD_DIR/compile_commands.json" ]; then
    red "${BOLD}[ERROR]: Compilation database (compile_commands.json) not found.${RESET}"
    red "${BOLD}         ScribbleLabApp Build Script exits with nonzero exit code.${RESET}"
    exit 1
fi

# Run clang-tidy
echo ""
bold "${ORANGE}==>${RESET} Running clang-tidy..."
find "$SOURCE_DIR" -name "*.cpp" -o -name "*.h" | xargs clang-tidy -p "$BUILD_DIR" 2>&1 || { red "${BOLD}[ERROR]: clang-tidy analysis failed${RESET}"; exit 1; }

if [ $? -ne 0 ]; then
    bold "${RED}[ERROR]: clang-tidy analysis failed${RESET}"
    echo "$clang_tidy_output"
    exit 1
fi

# Check if clang-tidy produced any warnings or errors
if [ -n "$clang_tidy_output" ]; then
    # Output the clang-tidy results
    echo "$clang_tidy_output"
    bold "${ORANGE}[WARNING]: clang-tidy found issues${RESET}"
    exit 1
fi

echo ""
bold "${GREEN}[SUCCESS]: Linting successful.${RESET}"


echo ""
bold "${GREEN}[SUCCESS]: Building project completed.${RESET}"
green "${GREEN}           Exiting build script with exit code 0. SUCESS"

exit 0
