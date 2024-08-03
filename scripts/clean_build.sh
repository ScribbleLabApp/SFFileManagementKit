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

# This script removes all CMake-generated files and directories for an Xcode build.

BUILD_DIR="../build"

GREEN='\033[1;32m'
RED='\033[0;31m'
ORANGE='\033[0;33m'
BLUE='\033[0;34m'
BOLD='\033[1m'
RESET='\033[0m'

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

bold "${BOLD}Welcome to the ScribbleLabApp SFFileManagementKit clean-up script${BOLD}"
echo ""
echo "Version: 0.1.0-beta (3)"
echo "Copyright (c) 2024 - ScribbleLabApp. All rights reserved."
echo ""

bold "${ORANGE}==>${RESET} Removing build directory..."

if [ -d "$BUILD_DIR" ]; then
    rm -rf "$BUILD_DIR"
else
    bold "${BLUE}==>${RESET} Build directory does not exist. Nothing to clean."
fi

bold "${ORANGE}==>${RESET} Removing CMake cache files..."
rm -f CMakeCache.txt
rm -rf CMakeFiles

bold "${GREEN}[SUCCESS]: Cleanup complete.${RESET}"
exit 0
