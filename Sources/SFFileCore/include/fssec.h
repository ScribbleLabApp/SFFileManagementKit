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

// Function prototypes
void* generate_key_iv(unsigned char *key, unsigned char *iv);
int encrypt_data(const unsigned char *plaintext, int plaintext_len, const unsigned char *key, const unsigned char *iv, unsigned char *ciphertext);
int decrypt_data(const unsigned char *ciphertext, int ciphertext_len, const unsigned char *key, const unsigned char *iv, unsigned char *plaintext);

#ifdef __cplusplus
}
#endif

#endif /* fssec_h */
