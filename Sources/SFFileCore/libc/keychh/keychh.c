//===-- libc/keychh/keychh.c - Keychain operations -------------  -*- C -*-===//
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

#include "keychh.h"
#include <CoreFoundation/CoreFoundation.h>
#include <Security/Security.h>
#include <string.h>

static char bundleIdentifier[256] = {0};

void setBundleIdentifier(const char* bundleID) {
    strncpy(bundleIdentifier, bundleID, sizeof(bundleIdentifier) - 1);
    bundleIdentifier[sizeof(bundleIdentifier) - 1] = '\0';
}

const char* getBundleIdentifier(void) {
    return bundleIdentifier;
}

int storeKeyInKeychain(const unsigned char* key, size_t keyLength, const char* keySuffix) {
    CFDataRef keyData = CFDataCreate(NULL, key, keyLength);
    if (!keyData) {
        return KEYCHH_ERR_CF_DATA_CREATE;
    }

    char identifierString[512];
    snprintf(identifierString, sizeof(identifierString), "%s.%s", bundleIdentifier, keySuffix);

    CFStringRef identifierCFString = CFStringCreateWithCString(NULL, identifierString, kCFStringEncodingUTF8);
    if (!identifierCFString) {
        CFRelease(keyData);
        return KEYCHH_ERR_CF_STRING_CREATE;
    }

    const void *keys[] = { kSecClass, kSecAttrApplicationTag, kSecValueData, kSecAttrAccessible };
    const void *values[] = { kSecClassKey, identifierCFString, keyData, kSecAttrAccessibleWhenUnlocked };
    CFDictionaryRef query = CFDictionaryCreate(NULL, keys, values, sizeof(keys) / sizeof(keys[0]), NULL, NULL);

    OSStatus status = SecItemAdd(query, NULL);

    CFRelease(keyData);
    CFRelease(identifierCFString);
    CFRelease(query);

    return (status == errSecSuccess);
}

CFDataRef retrieveKeyFromKeychain(const char* keySuffix) {
    char identifierString[512];
    snprintf(identifierString, sizeof(identifierString), "%s.%s", bundleIdentifier, keySuffix);

    CFStringRef identifierCFString = CFStringCreateWithCString(NULL, identifierString, kCFStringEncodingUTF8);
    if (!identifierCFString) {
        return NULL;
    }

    // Create query dictionary for retrieving the key
    const void *keys[] = { kSecClass, kSecAttrApplicationTag, kSecReturnData, kSecMatchLimit };
    const void *values[] = { kSecClassKey, identifierCFString, kCFBooleanTrue, kSecMatchLimitOne };
    CFDictionaryRef query = CFDictionaryCreate(NULL, keys, values, sizeof(keys) / sizeof(keys[0]), NULL, NULL);

    CFTypeRef result = NULL;
    OSStatus status = SecItemCopyMatching(query, &result);

    // Clean up
    CFRelease(identifierCFString);
    CFRelease(query);

    if (status == errSecSuccess) {
        return (CFDataRef)result;
    }
    return NULL;
}

void deleteKeyFromKeychain(const char* keySuffix) {
    char identifierString[512];
    snprintf(identifierString, sizeof(identifierString), "%s.%s", bundleIdentifier, keySuffix);

    CFStringRef key = CFStringCreateWithCString(NULL, identifierString, kCFStringEncodingUTF8);
    CFPreferencesSetAppValue(key, NULL, kCFPreferencesCurrentApplication);
    CFPreferencesAppSynchronize(kCFPreferencesCurrentApplication);
    CFRelease(key);
}