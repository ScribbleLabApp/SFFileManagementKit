//===-- SFFileCxxCore/include/fssec.h - File En-/Decryption ----  -*- C -*-===//
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

#ifndef fssec_h
#define fssec_h

#include <stdint.h>

#define AES_KEY_SIZE 256
#define AES_BLOCK_SIZE 16

#define SF_ERR_GENKEY -11           ///< Error code indicating key generation failure.
#define SF_ERR_ENCR -12             ///< Error code indicating encryption failure.
#define SF_ERR_DECR -13             ///< Error code indicating decryption failure.
#define SF_ERR_INIT -14             ///< Error code indicating initialization failure.
#define SF_ERR_OSSL -15             ///< Error code indicating an OpenSSL error.

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Generates a random AES key and initialization vector (IV).
 *
 * This function uses OpenSSL's RAND_bytes() function to generate a random AES key
 * and IV. The key and IV are stored in the provided buffers.
 *
 * \param key A pointer to a buffer where the generated AES key will be stored.
 *            The buffer must be at least AES_KEY_SIZE bytes long.
 * \param iv  A pointer to a buffer where the generated AES IV will be stored.
 *            The buffer must be at least AES_BLOCK_SIZE bytes long.
 *
 * \return 0 on success, or a negative error code on failure.
 *         - SF_ERR_GENKEY: Key generation failure.
 *         - SF_ERR_OSSL: An OpenSSL error occurred.
 */
int generate_key_iv(unsigned char *key, unsigned char *iv);

/**
 * \brief Encrypts a file using AES-256 in CBC mode with the provided key and IV.
 *
 * This function reads the input file, encrypts its contents using AES-256 in CBC mode,
 * and writes the encrypted data to the output file. The key and IV must be 256-bit and
 * 128-bit respectively.
 *
 * \param inputFilePath  A pointer to a null-terminated string representing the path to the input file.
 * \param outputFilePath A pointer to a null-terminated string representing the path to the output file.
 * \param key            A pointer to a buffer containing the 256-bit AES key.
 * \param iv             A pointer to a buffer containing the 128-bit AES IV.
 *
 * \return 0 on success, or a negative error code on failure.
 *         - SF_ERR_ENCR: Encryption failure.
 *         - SF_ERR_OSSL: An OpenSSL error occurred.
 */
int encrypt_file(const char* inputFilePath, const char* outputFilePath, const unsigned char* key, const unsigned char* iv);

/**
 * \brief Decrypts a file using AES-256 in CBC mode with the provided key and IV.
 *
 * This function reads the input file, decrypts its contents using AES-256 in CBC mode,
 * and writes the decrypted data to the output file. The key and IV must be 256-bit and
 * 128-bit respectively.
 *
 * \param inputFilePath  A pointer to a null-terminated string representing the path to the input file.
 *                       The input file must be a valid encrypted file created using the encrypt_file() function.
 * \param outputFilePath A pointer to a null-terminated string representing the path to the output file.
 *                       The decrypted data will be written to this file.
 * \param key            A pointer to a buffer containing the 256-bit AES key.
 *                       The key must be the same as the one used for encryption.
 * \param iv             A pointer to a buffer containing the 128-bit AES IV.
 *                       The IV must be the same as the one used for encryption.
 *
 * \return 0 on success, or a negative error code on failure.
 *         - SF_ERR_DECR: Decryption failure.
 *         - SF_ERR_OSSL: An OpenSSL error occurred.
 */
int decrypt_file(const char* inputFilePath, const char* outputFilePath, const unsigned char* key, const unsigned char* iv);

#ifdef __cplusplus
}
#endif

#endif /* fssec_h */
