//
//  keychh.c
//  SFFileManagementKit
//
//  Created by Nevio Hirani on 25.07.24.
//

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
