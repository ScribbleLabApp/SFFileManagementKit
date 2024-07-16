# This source file is part of the Scribble Foundation open source project
#
# Copyright (c) 2024 ScribbleLabApp. and the ScribbleLab project authors
# Licensed under Apache License v2.0 with Runtime Library Exception
#
# You may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Directories
OBJDIR := obj
OBJ_OBJC_DIR := $(OBJDIR)/objc
OBJ_LIBC_DIR := $(OBJ_OBJC_DIR)/libc
OBJ_LIBCXX_DIR := $(OBJ_OBJC_DIR)/libcxx/CompressionModule

# Compilation flags
CC := gcc
CXX := g++
CFLAGS := -I/opt/homebrew/opt/openssl@3/include -I./Sources/SFFileCxxCore/include -I./Sources/_SFCxxUtils/include -Wall -Wextra -pedantic
CXXFLAGS := $(CFLAGS) -std=c++17

# Source file directories
SRCDIR := Sources
LIBC_SRC_DIR := $(SRCDIR)/SFFileCxxCore/libc/fs
LIBCXX_SRC_DIR := $(SRCDIR)/SFFileCxxCore/libcxx/CompressionModule

# Object files
LIBC_OBJ := $(OBJ_LIBC_DIR)/SFCFileOperations.o
LIBCXX_OBJ := $(OBJ_LIBCXX_DIR)/SFCxxCompressionModule.o

# Default target
all: libc libcxx

# Compile C source files
libc: $(LIBC_OBJ)

$(OBJ_LIBC_DIR)/%.o: $(LIBC_SRC_DIR)/%.c | $(OBJ_LIBC_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile C++ source files
libcxx: $(LIBCXX_OBJ)

$(OBJ_LIBCXX_DIR)/%.o: $(LIBCXX_SRC_DIR)/%.cpp | $(OBJ_LIBCXX_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create directories if they don't exist
$(OBJ_LIBC_DIR):
	mkdir -p $@

$(OBJ_LIBCXX_DIR):
	mkdir -p $@

.PHONY: clean
clean:
	rm -rf $(OBJDIR)

