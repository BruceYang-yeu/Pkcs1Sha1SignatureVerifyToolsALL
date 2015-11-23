#ifndef _BOOTLOAD_VERIFIER_H
#define _BOOTLOAD_VERIFIER_H

#include "rsa.h"
#include "common.h"



typedef struct Certificate {
    int hash_len;  // SHA_DIGEST_SIZE (SHA-1) or SHA256_DIGEST_SIZE (SHA-256)
    RSAPublicKey* public_key;
} Certificate;

typedef enum { false, true }bool;

/* Look in the file for a signature footer, and verify that it
 * matches one of the given keys.  Return one of the constants below.
 */
int Verify_SHA1(const RSAPublicKey *key, unsigned char *inData, unsigned long inDataLen, unsigned char *pSignature, unsigned long pSigLen);
Certificate* load_keys(const char* filename, int* numKeys);
#endif // _BOOTLOAD_VERIFIER_H
