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

# Create the build directory
echo ""
bold "${ORANGE}==>${RESET} Creating build directory..."
mkdir "$BUILD_DIR"
cd "$BUILD_DIR" || { red "Failed to change directory to $BUILD_DIR"; exit 1; }

# Configure the project using CMake with Xcode generator
echo ""
bold "${ORANGE}==>${RESET} Configuring the project..."
cmake -G Xcode "$SOURCE_DIR" || { red "CMake configuration failed"; exit 1; } # cmake -G Xcode .. 

# Build the project using Xcode
echo ""
bold "${ORANGE}==>${RESET} Building the project..."
xcodebuild -project SFFileManagementKit.xcodeproj -scheme SFFileManagementKit || { red "Build failed"; exit 1; }

echo ""
bold "${GREEN}[SUCCESS]: Build complete.${RESET}"
