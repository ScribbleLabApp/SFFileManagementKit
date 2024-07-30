//===-- SFFileCore/include/libc/keychh.h - Keychain operations -  -*- C -*-===//
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

#ifndef keychh_h
#define keychh_h

#include <stdlib.h>
#include <Security/Security.h>

#define KEYCHH_ERR_CF_DATA_CREATE               -20
#define KEYCHH_ERR_CF_STRING_CREATE             -21
#define KEYCHH_ERR_KEYCHAIN_ADD_FAILED          -22
#define KEYCHH_ERR_KEYCHAIN_RETRIEVE_FAILED     -23
#define KEYCHH_ERR_KEY_NOT_FOUND                -24

/**
 * Sets the bundle identifier for the current application.
 *
 * @param bundleID A null-terminated C string representing the bundle identifier.
 *
 * This function updates the bundle identifier for the current application.
 * The bundle identifier is used to uniquely identify the application within the system.
 *
 * @note This function does not validate the input bundle identifier. It is the caller's responsibility to ensure that the provided bundle identifier is valid.
 */
void setBundleIdentifier(const char* bundleID);

/**
 * Retrieves the bundle identifier for the current application.
 *
 * @return A null-terminated C string representing the bundle identifier.
 *
 * This function retrieves the bundle identifier for the current application.
 * The bundle identifier is used to uniquely identify the application within the system.
 *
 * @note The returned bundle identifier is owned by the system and should not be modified or freed.
 *       If you need to store or use the bundle identifier, make a copy of it.
 */
const char* getBundleIdentifier(void);

/**
 * Stores a key in the keychain with a given suffix.
 *
 * @param key A pointer to an unsigned char array representing the key to be stored.
 * @param keyLength The length of the key in bytes.
 * @param keySuffix A null-terminated C string representing the suffix to be appended to the keychain item's label.
 *
 * This function stores a key in the keychain with a unique label constructed from the bundle identifier and the provided keySuffix.
 * The key is stored as a generic password item with the kSecAttrAccessible attribute set to kSecAttrAccessibleWhenUnlocked.
 *
 * @return An integer representing the status of the keychain operation.
 * - 0: The key was successfully stored in the keychain.
 * - Non-zero: An error occurred during the keychain operation.
 *
 * @note This function does not validate the input key or keySuffix. It is the caller's responsibility to ensure that the provided inputs are valid.
 */
int storeKeyInKeychain(const unsigned char* key, size_t keyLength, const char* keySuffix);

/**
 * Retrieves a key from the keychain with a given suffix.
 *
 * @param keySuffix A null-terminated C string representing the suffix to be appended to the keychain item's label.
 *
 * This function retrieves a key from the keychain with a unique label constructed from the bundle identifier and the provided keySuffix.
 * The key is retrieved as a generic password item with the kSecAttrAccessible attribute set to kSecAttrAccessibleWhenUnlocked.
 *
 * @return A CFDataRef representing the retrieved key.
 * - If the key is successfully retrieved, a CFDataRef containing the key's bytes is returned.
 * - If an error occurs during the keychain operation or if the key is not found, NULL is returned.
 *
 * @note This function does not validate the input keySuffix. It is the caller's responsibility to ensure that the provided input is valid.
 */
CFDataRef retrieveKeyFromKeychain(const char* keySuffix);

/**
 * Deletes a key from the keychain with a given suffix.
 *
 * @param keySuffix A null-terminated C string representing the suffix to be appended to the keychain item's label.
 *
 * This function deletes a key from the keychain with a unique label constructed from the bundle identifier and the provided keySuffix.
 * The key is identified as a generic password item with the kSecAttrAccessible attribute set to kSecAttrAccessibleWhenUnlocked.
 *
 * @note This function does not validate the input keySuffix. It is the caller's responsibility to ensure that the provided input is valid.
 *
 * @note If the key is not found in the keychain, this function does not return an error.
 */
void deleteKeyFromKeychain(const char* keySuffix);

#endif /* keychh_h */
