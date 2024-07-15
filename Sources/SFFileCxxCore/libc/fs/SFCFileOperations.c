//===-- libc/fs/SFCFileOperations.c - fs operations ------------  -*- C -*-===//
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
/// This source file declares functions for performing file operations such as
/// writing, reading, deleting, and opening files containing JSON data. It
/// includes C function declarations and an interface for encoding and decoding
/// JSON data, compatible with C++ implementations.
///
//===----------------------------------------------------------------------===//

#include "SFCFileOperations.h"

int writeScribbleFile(const char* filePath, void* jsonVariant) {
    int fd = open(filePath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        return -1;
    }
    
    char* jsonString = encodeToJSON(jsonVariant);
    if (!jsonString) {
        return -1;
    }
    
    size_t jsonStringLength = strlen(jsonString);
    ssize_t written = write(fd, jsonString, jsonStringLength);
    free(jsonString);
    close(fd);
    
    if (written == -1 || (size_t)written != jsonStringLength) {
        return -1;
    }
    
    return 0;
}

void* readScribbleFile(const char* filePath) {
    int fd = open(filePath, O_RDONLY);
    if (fd == -1) {
        return NULL;
    }
    
    off_t fileSize = lseek(fd, 0, SEEK_END);
    if (fileSize == -1) {
        close(fd);
        return NULL;
    }
    
    lseek(fd, 0, SEEK_SET);
    
    char* jsonString = (char*)malloc(fileSize + 1);
    if (!jsonString) {
        close(fd);
        return NULL;
    }
    
    size_t readSize = read(fd, jsonString, fileSize);
    close(fd);
    
    if (readSize == -1) {
        free(jsonString);
        return NULL;
    }
    
    jsonString[readSize] = '\0';
    void* jsonVariant = decodeFromJSON(jsonString);
    free(jsonString);
    
    return jsonVariant;
}

int deleteScribbleFile(const char* filePath) {
    return unlink(filePath);
}

int openScribbleFile(const char* filePath, int flags) {
    int fd = open(filePath, flags);
    if (fd == -1) {
        return -1;
    }
    
    return fd;
}
