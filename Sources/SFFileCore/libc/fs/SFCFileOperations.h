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
#include <errno.h>
#include <sys/stat.h>

#include "SFCJSON.h"

#define SFC_SUCCESS 0                       ///< Error code indicating success.
#define SFC_FAILURE -1                      ///< Error code indicating general failure.
#define SFC_ERR_MEMORY -2                   ///< Error code indicating insufficient memory.
#define SFC_ERR_FILE_NOT_FOUND -3           ///< Error code indicating file not found.
#define SFC_ERR_PERMISSION_DENIED -4        ///< Error code indicating permission denied.
#define SFC_ERR_FILE_EXSISTS -5             ///< Error code indicating file already exists.
#define SFC_ERR_INVALID_ARGS -6             ///< Error code indicating invalid arguments.
#define SFC_ERR_IO -7                       ///< Error code indicating I/O error.
#define SFC_ERR_READ -8                     ///< Error code indicating failure during read operation.
#define SFC_ERR_WRITE -9                    ///< Error code indicating failure during write operation.
#define SFC_ERR_UNKNOWN -10                 ///< Error code indicating unexpected error.

#define SFC_MASK_READ 0x01                  ///< Mask to check read permission.
#define SFC_MASK_WRITE 0x02                 ///< Mask to check write permission.
#define SFC_MASK_EXECUTE 0x04               ///< Mask to check execute permission.

#define SFC_FLAG_READ O_RDONLY              ///< Flag to open file for reading.
#define SFC_FLAG_WRITE O_WRONLY             ///< Flag to open file for writing.
#define SFC_FLAG_READWRITE O_RDWR           ///< Flag to open file for reading and writing.

#define CHECK_NULL(ptr) if ((ptr) == NULL) {       \
    fprintf(stderr, "Memory allocation failed\n"); \
    return NULL;                                   \
}                                                  \

/// \brief Writes a JSON boiler plate to a JSONVariant object.
///
/// This function creates a JSONVariant object with a predefined structure,
/// representing a basic JSON boiler plate. The JSONVariant object can then be
/// encoded to a JSON string for further processing.
///
/// \return A JSONVariant object containing the predefined JSON boiler plate.
///         The caller is responsible for freeing the JSONVariant object.
JSONVariant writeJSONBoilerPlate();

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

#pragma mark - Helper functions start


/// \brief Creates a directory at the specified path.
/// 
/// This function creates a directory at the given path.
/// 
/// \param path The path where the directory should be created.
/// \return 0 if the directory is created successfully, -1 otherwise.
int createDirectory(const char* path);

/// \brief Configures the specified configuration file within the .scribble archive.
///
/// This function configures the specified configuration file within the .scribble archive.
///
/// \param archivePath The path to the .scribble archive.
/// \param filePath The path to the configuration file within the archive.
/// \return 0 on success, SFC_ERR_FILE_NOT_FOUND (-3) if the archive or file does not exist,
///         SFC_ERR_PERMISSION_DENIED (-4) if permission is denied.
int configConfigFile(const char* archivePath, const char* filePath);

/// \brief Creates the initial directories for the given path. (DEPRECATED in 0.1.0-beta.1)
///
/// This function creates the necessary directories for the given path if they do not already exist.
///
/// \param path The path for which to create the initial directories.
/// \return Returns 0 if the directories are created successfully, or a negative value if an error occurs.
static int createInitialDirectories(const char* path) __attribute__((deprecated));

#pragma mark - Helper functions end

/// Creates a new .scribble archive with the necessary directory structure.
///
/// This function creates a new .scribble archive at the specified path, initializing
/// the archive with the directories img/vec, txt, and temp, and an empty .scconfig file.
///
/// \param archivePath The path where the .scribble archive will be created.
/// \return 0 on success, SFC_ERR_MEMORY (-2) if memory allocation fails, SFC_ERR_FILE_EXISTS (-5) if the file already exists.
int createScribbleArchive(const char* archivePath);

/// Deletes the specified .scribble archive.
///
/// \param archivePath The path to the .scribble archive to delete.
/// \return 0 on success, SFC_ERR_FILE_NOT_FOUND (-3) if the archive does not exist, SFC_ERR_PERMISSION_DENIED (-4) if permission is denied.
int deleteScribbleArchive(const char* archivePath);

/// Opens the .scribble archive with the specified flags.
///
/// \param archivePath The path to the .scribble archive.
/// \param flags The flags for opening the archive (e.g., O_RDONLY, O_WRONLY).
/// \return File descriptor on success, SFC_ERR_FILE_NOT_FOUND (-3) if the archive does not exist, 
///         SFC_ERR_PERMISSION_DENIED (-4) if permission is denied.
int openScribbleArchive(const char* archivePath, int flags);


/// Writes JSON content to the specified configuration file within the .scribble archive.
///
/// \param archivePath The path to the .scribble archive.
/// \param filePath The path to the configuration file within the archive.
/// \param jsonContent The JSON content to be written to the configuration file.
/// \return 0 on success, SFC_ERR_FILE_NOT_FOUND (-3) if the archive or file does not exist,
///         SFC_ERR_PERMISSION_DENIED (-4) if permission is denied, SFC_ERR_WRITE (-9) on write failure
int writeConfigFile(const char* archivePath, const char* filePath, const char* jsonContent);

/// Reads the JSON content from the specified configuration file within the .scribble archive.
///
/// \param archivePath The path to the .scribble archive.
/// \param filePath The path to the configuration file within the archive.
/// \return The JSON content as a string, or NULL on failure. The caller is responsible for freeing the returned string.
///         Returns NULL and sets errno to SFC_ERR_FILE_NOT_FOUND (-3) if the archive or file does not exist, SFC_ERR_READ (-8) on read failure.
char* readConfigFile(const char* archivePath, const char* filePath);

/// Opens the specified configuration file within the .scribble archive with the given flags.
///
/// \param archivePath The path to the .scribble archive.
/// \param filePath The path to the configuration file within the archive.
/// \param flags The flags for opening the file (e.g., O_RDONLY, O_WRONLY).
/// \return File descriptor on success, SFC_ERR_FILE_NOT_FOUND (-3) if the archive or file does not exist, SFC_ERR_PERMISSION_DENIED (-4) if permission is denied.
int openConfigFile(const char* archivePath, const char* filePath, int flags);


/// Writes text content to the specified text file within the .scribble archive.
///
/// \param archivePath The path to the .scribble archive.
/// \param filePath The path to the text file within the archive.
/// \param txtContent The text content to be written to the text file.
/// \return 0 on success, SFC_ERR_FILE_NOT_FOUND (-3) if the archive or file does not exist, SFC_ERR_PERMISSION_DENIED (-4) if permission is denied, SFC_ERR_WRITE (-9) on write failure.
int writeTxtFile(const char* archivePath, const char* filePath, const char* txtContent);

/// Reads the text content from the specified text file within the .scribble archive.
///
/// \param archivePath The path to the .scribble archive.
/// \param filePath The path to the text file within the archive.
/// \return The text content as a string, or NULL on failure. The caller is responsible for freeing the returned string.
///         Returns NULL and sets errno to SFC_ERR_FILE_NOT_FOUND (-3) if the archive or file does not exist,, SFC_ERR_READ (-8) on read failure.
char* readTxtFile(const char* archivePath, const char* filePath);

/// Opens the specified text file within the .scribble archive with the given flags.
///
/// \param archivePath The path to the .scribble archive.
/// \param filePath The path to the text file within the archive.
/// \param flags The flags for opening the file (e.g., O_RDONLY, O_WRONLY).
/// \return File descriptor on success, SFC_ERR_FILE_NOT_FOUND (-3) if the archive or file does not exist, SFC_ERR_PERMISSION_DENIED  (-4)if permission is denied.
int openTxtFile(const char* archivePath, const char* filePath, int flags);


/// Represents a file with metadata and content.
///
/// This struct is used to store information about a file, including whether it is open,
/// whether it is writable, touched bits, and the file's data. It is used to manage
/// file operations within the ScribbleLab project.
///
/// \param isOpen Indicates if the file is currently open.
/// \param isWritable Indicates if the file is writable.
/// \param touchedBits A pointer to a buffer storing touched bits (metadata).
/// \param touchedSize The size of the touched bits buffer.
/// \param data A pointer to the file's data.
/// \param allocSize The allocated size of the data buffer.
/// \param size The actual size of the data.
typedef struct file {
    _Bool isOpen;
    _Bool isWritable;
    uint8_t* touchedBits;
    int touchedSize;
    char* data;
    int allocSize;
    int size;
} file_t;

#endif /* SFCFileOperations_h */
