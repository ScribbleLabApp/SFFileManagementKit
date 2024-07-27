//===-- SFFileCxxCore/libc/fssec.h - File En-/Decryption -------  -*- C -*-===//
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
///
//===----------------------------------------------------------------------===//

#include "fssec.h"
#include "keychh.h"
#include "SFCFileOperations.h"
#include <stdio.h>
#include <stdlib.h>
#include <Security/Security.h>
#include <CommonCrypto/CommonCrypto.h>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/rand.h>

int generate_key_iv(unsigned char *key, unsigned char *iv) {
    if (!RAND_bytes(key, AES_KEY_SIZE)) {
        return SF_ERR_GENKEY;
    }
    if (!RAND_bytes(iv, AES_BLOCK_SIZE)) {
        return SF_ERR_GENKEY;
    }
    return 0;
}

int encrypt_file(const char* inputFilePath, const char* outputFilePath, const unsigned char* key, const unsigned char* iv) {
    FILE* inputFile = fopen(inputFilePath, "rb");
    FILE* outputFile = fopen(outputFilePath, "wb");
    if (!inputFile || !outputFile) {
        perror("Error opening file");
        return SF_ERR_INIT;
    }

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        perror("Error creating cipher context");
        fclose(inputFile);
        fclose(outputFile);
        return SF_ERR_INIT;
    }

    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv) != 1) {
        perror("Error initializing encryption");
        EVP_CIPHER_CTX_free(ctx);
        fclose(inputFile);
        fclose(outputFile);
        return SF_ERR_ENCR;
    }

    unsigned char buffer[AES_BLOCK_SIZE];
    unsigned char ciphertext[AES_BLOCK_SIZE + EVP_MAX_BLOCK_LENGTH];
    int bytesRead, ciphertextLen;

    while ((bytesRead = fread(buffer, 1, AES_BLOCK_SIZE, inputFile)) > 0) {
        if (EVP_EncryptUpdate(ctx, ciphertext, &ciphertextLen, buffer, bytesRead) != 1) {
            perror("Error encrypting data");
            EVP_CIPHER_CTX_free(ctx);
            fclose(inputFile);
            fclose(outputFile);
            return SF_ERR_ENCR;
        }
        fwrite(ciphertext, 1, ciphertextLen, outputFile);
    }

    if (EVP_EncryptFinal_ex(ctx, ciphertext, &ciphertextLen) != 1) {
        perror("Error finalizing encryption");
        EVP_CIPHER_CTX_free(ctx);
        fclose(inputFile);
        fclose(outputFile);
        return SF_ERR_ENCR;
    }
    fwrite(ciphertext, 1, ciphertextLen, outputFile);

    EVP_CIPHER_CTX_free(ctx);
    fclose(inputFile);
    fclose(outputFile);

    return 0;
}

int decrypt_file(const char* inputFilePath, const char* outputFilePath, const unsigned char* key, const unsigned char* iv) {
    FILE* inputFile = fopen(inputFilePath, "rb");
    FILE* outputFile = fopen(outputFilePath, "wb");
    if (!inputFile || !outputFile) {
        perror("Error opening file");
        return SF_ERR_INIT;
    }

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        perror("Error creating cipher context");
        fclose(inputFile);
        fclose(outputFile);
        return SF_ERR_INIT;
    }

    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv) != 1) {
        perror("Error initializing decryption");
        EVP_CIPHER_CTX_free(ctx);
        fclose(inputFile);
        fclose(outputFile);
        return SF_ERR_DECR;
    }

    unsigned char buffer[AES_BLOCK_SIZE];
    unsigned char plaintext[AES_BLOCK_SIZE + EVP_MAX_BLOCK_LENGTH];
    int bytesRead, plaintextLen;

    while ((bytesRead = fread(buffer, 1, AES_BLOCK_SIZE, inputFile)) > 0) {
        if (EVP_DecryptUpdate(ctx, plaintext, &plaintextLen, buffer, bytesRead) != 1) {
            perror("Error decrypting data");
            EVP_CIPHER_CTX_free(ctx);
            fclose(inputFile);
            fclose(outputFile);
            return SF_ERR_DECR;
        }
        fwrite(plaintext, 1, plaintextLen, outputFile);
    }

    if (EVP_DecryptFinal_ex(ctx, plaintext, &plaintextLen) != 1) {
        perror("Error finalizing decryption");
        EVP_CIPHER_CTX_free(ctx);
        fclose(inputFile);
        fclose(outputFile);
        return SF_ERR_DECR;
    }
    fwrite(plaintext, 1, plaintextLen, outputFile);

    EVP_CIPHER_CTX_free(ctx);
    fclose(inputFile);
    fclose(outputFile);

    return 0;
}

int decryptScribbleArchive(const char* archivePath, char* tempPath) {
    int fd = open(archivePath, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open archive - SCF_ERR_IO");
        return SFC_ERR_IO;
    }

    off_t fileSize = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    unsigned char* encryptedData = (unsigned char*)malloc(fileSize);
    if (read(fd, encryptedData, fileSize) != fileSize) {
        perror("Failed to read the file - SFC_ERR_READ");
        close(fd);
        free(encryptedData);
        return SFC_ERR_READ;
    }
    close(fd);

    CFDataRef keyData = retrieveKeyFromKeychain("key");
    CFDataRef ivData = retrieveKeyFromKeychain("iv");

    if (keyData == NULL || ivData == NULL) {
        fprintf(stderr, "Failed to retrieve key or IV from keychain - KEYCHH_ERR_KEYCHAIN_RETRIEVE_FAILED\n");
        if (keyData) CFRelease(keyData);
        if (ivData) CFRelease(ivData);
        free(encryptedData);
        return KEYCHH_ERR_KEYCHAIN_RETRIEVE_FAILED;
    }

    unsigned char* decryptedData = (unsigned char*)malloc(fileSize);
    size_t decryptedDataLen = 0;
    CCCryptorStatus cryptStatus = CCCrypt(
        kCCDecrypt,
        kCCAlgorithmAES128,
        kCCOptionPKCS7Padding,
        CFDataGetBytePtr(keyData),
        kCCKeySizeAES128,
        CFDataGetBytePtr(ivData),
        encryptedData,
        fileSize,
        decryptedData,
        fileSize,
        &decryptedDataLen
    );

    CFRelease(keyData);
    CFRelease(ivData);
    free(encryptedData);

    if (cryptStatus != kCCSuccess) {
        fprintf(stderr, "Failed to decrypt the archive - SF_ERR_DECR\n");
        free(decryptedData);
        return SF_ERR_DECR;;
    }

    int tempFd = mkstemp(tempPath);
    if (tempFd == -1) {
        perror("Failed to create temporary file - SFC_ERR_IO");
        free(decryptedData);
        return SFC_ERR_IO;
    }

    if (write(tempFd, decryptedData, decryptedDataLen) != decryptedDataLen) {
        perror("Failed to write to temporary file - SFC_ERR_WRITE");
        close(tempFd);
        free(decryptedData);
        return SFC_ERR_WRITE;
    }

    free(decryptedData);
    close(tempFd);

    return SFC_SUCCESS;
}

int encryptScribbleArchive(const char* archivePath, const char* tempPath) {
    int fd = open(tempPath, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open temporary file - SFC_ERR_IO");
        return SFC_ERR_IO;
    }

    off_t fileSize = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    unsigned char* decryptedData = (unsigned char*)malloc(fileSize);
    if (read(fd, decryptedData, fileSize) != fileSize) {
        perror("Failed to read the temporary file - SFC_ERR_READ");
        close(fd);
        free(decryptedData);
        return SFC_ERR_READ;
    }
    close(fd);

    CFDataRef keyData = retrieveKeyFromKeychain("key");
    CFDataRef ivData = retrieveKeyFromKeychain("iv");

    if (keyData != NULL || ivData == NULL) {
        fprintf(stderr, "Failed to retrieve key or IV from keychain - KEYCHH_ERR_KEYCHAIN_RETRIEVE_FAILED\n");
        if (keyData) CFRelease(keyData);
        if (ivData) CFRelease(ivData);
        free(decryptedData);
        return KEYCHH_ERR_KEYCHAIN_RETRIEVE_FAILED;
    }

    unsigned char* encryptedData = (unsigned char*)malloc(fileSize + kCCBlockSizeAES128);
    size_t encryptedDataLen = 0;
    CCCryptorStatus cryptStatus = CCCrypt(
        kCCEncrypt,
        kCCAlgorithmAES128,
        kCCOptionPKCS7Padding,
        CFDataGetBytePtr(keyData),
        kCCKeySizeAES128,
        CFDataGetBytePtr(ivData),
        decryptedData,
        fileSize,
        encryptedData,
        fileSize + kCCBlockSizeAES128,
        &encryptedDataLen
    );

    CFRelease(keyData);
    CFRelease(ivData);
    free(decryptedData);

    if (cryptStatus != kCCSuccess) {
        fprintf(stderr, "Failed to encrypt the archive - SF_ERR_ENCR\n");
        free(encryptedData);
        return SF_ERR_ENCR;
    }

    int archiveFd = open(archivePath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (archiveFd == -1) {
        perror("Failed to open archive file - SFC_ERR_IO");
        free(encryptedData);
        return SFC_ERR_IO;
    }

    if (write(archiveFd, encryptedData, encryptedDataLen) != encryptedDataLen) {
        perror("Failed to write to archive file - SFC_ERR_WRITE");
        close(archiveFd);
        free(encryptedData);
        return SFC_ERR_WRITE;
    }

    free(encryptedData);
    close(archiveFd);

    return SFC_SUCCESS;
}