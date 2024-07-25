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

#include "fssec.h"
#include "SFCJSON.h"
#include "SFCFileOperations.h"

#include "keychh.h"

#include <errno.h>
#include <sys/stat.h>
#include <openssl/rand.h>

static ConfigArgs g_configArgs;

#pragma mark - Config Arguments functions start

void setConfigData(const ConfigArgs* configArgs) {
    memcpy(&g_configArgs, configArgs, sizeof(ConfigArgs));
}

const ConfigArgs* getConfigData(void) {
    return &g_configArgs;
}

#pragma mark - Config Arguments functions end

#pragma mark - Helper functions start

JSONVariant writeJSONBoilerPlate(void) {
    const ConfigArgs* configArgs = getConfigData();
    
    JSONVariant project = json_create_object();
    CHECK_NULL(project);
    json_set_string(project, "name", configArgs->name);
    json_set_string(project, "author", configArgs->author);
    json_set_string(project, "created_at", configArgs->created_at);
    json_set_string(project, "last_changed_at", configArgs->last_changed_at);
    json_set_string(project, "editor_version", configArgs->editor_version);

    JSONVariant document_settings = json_create_object();
    CHECK_NULL(document_settings);
    json_set_string(document_settings, "encoding", configArgs->encoding);
    json_set_string(document_settings, "line_endings", configArgs->line_endings);

    JSONVariant security = json_create_object();
    CHECK_NULL(security);
    json_set_boolean(security, "password_protected", configArgs->psw_pr);
    json_set_string(security, "encryption_method", configArgs->encryption_method);

    JSONVariant flags = json_create_object();
    CHECK_NULL(flags);
    json_set_boolean(flags, "is_Favorite", configArgs->is_Favorite);

    JSONVariant references = json_create_object();
    CHECK_NULL(references);

    JSONVariant images = json_create_array();
    CHECK_NULL(images);

    JSONVariant text_files = json_create_array();
    CHECK_NULL(text_files);

    JSONVariant temporary = json_create_array();
    CHECK_NULL(temporary);

    JSONVariant root = json_create_object();
    CHECK_NULL(root);
    json_set_object(root, "project", project);
    json_set_object(root, "document_settings", document_settings);
    json_set_object(root, "security", security);
    json_set_object(root, "flags", flags);
    json_set_object(root, "references", references);

    // Free individual JSON variants if necessary
    // free_json(project);
    // free_json(document_settings);
    // free_json(security);
    // free_json(flags);
    // free_json(references);
    // free_json(images);
    // free_json(text_files);
    // free_json(temporary);

    return root;
}

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

    free(jsonContent);

    return SFC_SUCCESS;
}

#pragma mark - Helper functions end

int createScribbleArchive(const char* archivePath) {
    char imgVecPath[256];
    char txtPath[256];
    char tempPath[256];
    char configFilePath[256];

    char encryptedConfigFilePath[256];

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

    int configResult = configureConfigFile(archivePath, configFilePath);
    if (configResult != 0) {
        fclose(configFile);
        return configResult;
    }

    if (fclose(configFile) != 0) {
        perror("An error occurred while closing the .scconfig file");
        return SFC_ERR_IO;
    }

    unsigned char key[AES_KEY_SIZE / 8];
    unsigned char iv [AES_BLOCK_SIZE];

    if (!RAND_bytes(key, sizeof(key)) || !RAND_bytes(iv, sizeof(iv))) {
        perror("An error occurred while generating a key or iv");
        return SF_ERR_GENKEY;
    }

    if (encrypt_file(configFilePath, encryptedConfigFilePath, key, iv) != 0) {
        perror("An error occurred while encrypting the .scconfig file");
        return SF_ERR_ENCR;
    }

    int keyResult = storeKeyInKeychain(key, sizeof(key), "key");
    if (keyResult != 0) {
        perror("An error occurred while storing the key in the keychain");
        return keyResult;
    }

    int ivResult = storeKeyInKeychain(iv, sizeof(iv), "iv");
    if (keyResult != 0) {
        perror("An error occurred while storing the IV in the keychain");
        return ivResult;
    }

    return SFC_SUCCESS;
}

int deleteScribbleArchive(const char* archivePath) {
    if (archivePath == NULL) {
        perror("Invalid archive path - SFC_ERR_INVALID_ARG");
        return SFC_ERR_FILE_NOT_FOUND;
    }

    CFDataRef keyData = retrieveKeyFromKeychain("key");
    CFDataRef ivData = retrieveKeyFromKeychain("iv");

    if (keyData == NULL || ivData == NULL) {
        perror("An error occurred while retrieving key or iv from keychain");
        if (keyData) CFRelease(keyData);
        if (ivData) CFRelease(ivData);
        return KEYCHH_ERR_KEY_NOT_FOUND;
    }

    deleteKeyFromKeychain("key");
    deleteKeyFromKeychain("iv");

    CFRelease(keyData);
    CFRelease(ivData);

    if (remove(archivePath) == 0) {
        printf("The archive was deleted successfully.\n");
        return SFC_SUCCESS;
    } else {
        perror("An error occurred while deleting the file");
        return SFC_ERR_UNKNOWN;
    }
}

int openScribbleArchive(const char* archivePath, int flags) {}

int writeConfigFile(const char* archivePath, const char* filePath, const char* jsonContent) {}

char* readConfigFile(const char* archivePath, const char* filePath) {}

int openConfigFile(const char* archivePath, const char* filePath, int flags) {}

int writeTxtFile(const char* archivePath, const char* filePath, const char* txtContent) {}

char* readTxtFile(const char* archivePath, const char* filePath) {}

int openTxtFile(const char* archivePath, const char* filePath, int flags) {}
