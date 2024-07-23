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
#include "fssec.h"

//     __ _____ _____ _____                                                   //
//  __|  |   __|     |   | |  SFCxxJSON methods for Scribble Foundation       //
// |  |  |__   |  |  | | | |  Version 1.0                                     //
// |_____|_____|_____|_|___|  https://github.com/ScribbleLabApp/              //
//                                                                            //
// #include "SFCxxJSON.h"

#include <sys/stat.h>
#include <errno.h>

#pragma mark - Helper functions start

int createDirectory(const char* path) {
    if (mkdir(path, 0777) != 0) {
        if (errno != EEXIST) {
            perror("Directory already exists - SFC_ERR_FILE_EXSISTS");
            return SFC_ERR_FILE_EXSISTS;
        }

        perror("An error occurred while creating directory - SFC_ERR_IO");
        return SFC_ERR_IO;
    }

    return SFC_SUCCESS;
}

char* writeJSONBoilerPlate() {
    // TODO: Implement this function to return JSON content for the config file.
    // This is a placeholder implementation.
    return ""; // Return an empty JSON object as a placeholder
}

int configureConfigFile(const char* archivePath, const char* filePath) {
    int openResult = openConfigFile(archivePath, filePath, SFC_FLAG_READWRITE);
    if (openResult != SFC_SUCCESS) {
        perror("An error occurred while opening config file - SFC_ERR_IO");
        return openResult;
    }

    char* jsonContent = writeJSONBoilerPlate();
    if (jsonContent == NULL) {
        perror("An error occurred while generating JSON content - SFC_ERR_IO");
        return SFC_ERR_IO;
    }

    int writeResult = writeConfigFile(archivePath, filePath, jsonContent);
    if (writeResult != SFC_SUCCESS) {
        perror("An error occurred while writing to config file - SFC_ERR_WRITE");
        return writeResult; // ~> SFC_ERR_WRITE
    }

    return SFC_SUCCESS;
}

static int createInitialDirectories(const char* path) { return 0; }

#pragma mark - Helper functions end

int createScribbleArchive(const char* archivePath) {
    char imgVecPath[256];
    char txtPath[256];
    char tempPath[256];
    char configFilePath[256];

    snprintf(imgVecPath, sizeof(imgVecPath), "%s/img/vec", archivePath);
    snprintf(txtPath, sizeof(txtPath), "%s/txt", archivePath);
    snprintf(tempPath, sizeof(tempPath), "%s/temp", archivePath);
    snprintf(configFilePath, sizeof(configFilePath), "%s/.scconfig", archivePath);

    if (createDirectory(archivePath) != 0 || 
        createDirectory(imgVecPath) != 0 || 
        createDirectory(txtPath) != 0 || 
        createDirectory(tempPath) != 0) {
        return SFC_ERR_IO;
    }

    FILE* configFile = fopen(configFilePath, "w");
    if (!configFile) {
        perror("An error occurred while creating the .scconfig file");
        return SFC_ERR_IO;
    }

    // TODO: Set up .scconfig file

    configureConfigFile(archivePath, configFilePath);

    if (fclose(configFile) != 0) {
        perror("An error occurred while closing the .scconfig file");
        return SFC_ERR_IO;
    }

    // TODO: Encrypt and secure the archive

    return SFC_SUCCESS;
}

int deleteScribbleArchive(const char* archivePath) {}

int openScribbleArchive(const char* archivePath, int flags) {}

int writeConfigFile(const char* archivePath, const char* filePath, const char* jsonContent) {}

char* readConfigFile(const char* archivePath, const char* filePath) {}

int openConfigFile(const char* archivePath, const char* filePath, int flags) {}

int writeTxtFile(const char* archivePath, const char* filePath, const char* txtContent) {}

char* readTxtFile(const char* archivePath, const char* filePath) {}

int openTxtFile(const char* archivePath, const char* filePath, int flags) {}
