//
//  Header.h
//  
//
//  Created by Nevio Hirani on 16.07.24.
//

#ifndef fssec_h
#define fssec_h

#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <string.h>

#define AES_KEY_SIZE 256
#define AES_BLOCK_SIZE 16

#define SF_ERR_GENKEY -11           ///< Error code indicating key generation failure.
#define SF_ERR_ENCR -12             ///< Error code indicating encryption failure.
#define SF_ERR_DECR -13             ///< Error code indicating decryption failure.
#define SF_ERR_INIT -14             ///< Error code indicating initialization failure.
#define SF_ERR_OSSL -15             ///< Error code indicating an OpenSSL error.

int generate_key_iv(unsigned char *key, unsigned char *iv) {
    
}

#endif /* fssec_h */
