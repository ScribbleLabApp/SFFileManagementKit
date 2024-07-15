//===-- libc/fs/SFCFileOperations.h - fs operations ------------  -*- C -*-===//
//                                                                            //
// This source file is part of the Scribble Foundation open source project    //
//                                                                            //
// Copyright (c) 2024 ScribbleLabApp. and the ScribbleLab project authors     //
// Licensed under Apache License v2.0 with Runtime Library Exception          //
//                                                                            //
// You may not use this file except in compliance with the License.           //
// You may obtain a copy of the License at                                    //
//                                                                            //
//      http://www.apache.org/licenses/LICENSE-2.0                            //
//                                                                            //
// Unless required by applicable law or agreed to in writing, software        //
// distributed under the License is distributed on an "AS IS" BASIS,          //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   //
// See the License for the specific language governing permissions and        //
// limitations under the License.                                             //
//                                                                            //
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Provides file system operations for ScribbleLab files in JSON format.
///
/// This header file declares functions for performing file operations such as
/// writing, reading, deleting, and opening files containing JSON data. It
/// includes C function declarations and an interface for encoding and decoding
/// JSON data, compatible with C++ implementations.
///
//===----------------------------------------------------------------------===//

#ifndef SFCFileOperations_h
#define SFCFileOperations_h

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/// Encodes a JSONVariant to a JSON string.
///
/// This function takes a JSONVariant object and encodes it into a JSON formatted
/// string. The JSONVariant should be managed by the C++ implementation.
///
/// \param jsonVariant A pointer to the JSONVariant object to encode.
/// \return A dynamically allocated JSON string. The caller is responsible for freeing
///         this string.
char* encodeToJSON(void* jsonVariant);

/// Decodes a JSON string to a JSONVariant.
///
/// This function takes a JSON formatted string and decodes it into a JSONVariant
/// object. The JSONVariant should be managed by the C++ implementation.
///
/// \param jsonString A pointer to the JSON string to decode.
/// \return A pointer to the decoded JSONVariant object.
void* decodeFromJSON(const char* jsonString);

#ifdef __cplusplus
}
#endif

/// Writes a JSONVariant to a file.
///
/// This function writes a JSONVariant to the specified file in JSON format. If the
/// file does not exist, it will be created. If the file exists, its content will be
/// truncated before writing.
///
/// \param filePath The path of the file to write.
/// \param jsonVariant A pointer to the JSONVariant object to write.
/// \return 0 on success, -1 on failure.
int writeScribbleFile(const char* filePath, void* jsonVariant);

/// Reads a JSONVariant from a file.
///
/// This function reads the content of the specified file, which is expected to be
/// in JSON format, and decodes it into a JSONVariant object.
///
/// \param filePath The path of the file to read.
/// \return A pointer to the decoded JSONVariant object on success, NULL on failure.
void* readScribbleFile(const char* filePath);

/// Deletes a file.
///
/// This function deletes the specified file from the file system.
///
/// \param filePath The path of the file to delete.
/// \return 0 on success, -1 on failure.
int deleteScribbleFile(const char* filePath);

/// Opens a file with specified flags.
///
/// This function opens a file with the given flags and returns the file descriptor.
/// The flags parameter should be specified using the constants defined in <fcntl.h>.
///
/// \param filePath The path of the file to open.
/// \param flags The flags for opening the file (e.g., O_RDONLY, O_WRONLY).
/// \return The file descriptor on success, -1 on failure.
int openScribbleFile(const char* filePath, int flags);

#endif /* SFCFileOperations_h */
