/*
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "common.h"
#include "verifier.h"

#include "rsa.h"
#include "sha.h"

#include <string.h>
#include <stdio.h>
#include <errno.h>

// Look for an RSA signature embedded in origin buffer comment given
// Verify it matches one of the given public
// keys.
//
// Return VERIFY_SUCCESS, VERIFY_FAILURE (if any error is encountered
// or no key matches the signature).

int Verify_SHA1(const RSAPublicKey *key,
           unsigned char *inData, 
           unsigned long inDataLen, 
           unsigned char *pSignature, 
           unsigned long pSigLen)
{
    unsigned char SHA1digest[SHA_DIGEST_LENGTH]; /* holds digests */
    int status;                                  /* status code for RSA_sign function, returned value */
    
    memset(SHA1digest, 0, sizeof(SHA1digest));
    SHA_hash(inData, inDataLen, SHA1digest);

    status = RSA_verify(key, pSignature, pSigLen, SHA1digest, SHA_DIGEST_LENGTH);
    
    return status;
}

// Reads a file containing one or more public keys as produced by
// DumpPublicKey:  this is an RSAPublicKey struct as it would appear
// as a C source literal, eg:
//
//  "{64,0xc926ad21,{1795090719,...,-695002876},{-857949815,...,1175080310}}"
//
// For key versions newer than the original 2048-bit e=3 keys
// supported by Android, the string is preceded by a version
// identifier, eg:
//
//  "v2 {64,0xc926ad21,{1795090719,...,-695002876},{-857949815,...,1175080310}}"
//
// (Note that the braces and commas in this example are actual
// characters the parser expects to find in the file; the ellipses
// indicate more numbers omitted from this example.)
//
// The file may contain multiple keys in this format, separated by
// commas.  The last key must not be followed by a comma.
//
// A Certificate is a pair of an RSAPublicKey and a particular hash
// (we support SHA-1 and SHA-256; we store the hash length to signify
// which is being used).  The hash used is implied by the version number.
//
//       1: 2048-bit RSA key with e=3 and SHA-1 hash
//       2: 2048-bit RSA key with e=65537 and SHA-1 hash
//       3: 2048-bit RSA key with e=3 and SHA-256 hash
//       4: 2048-bit RSA key with e=65537 and SHA-256 hash
//
// Returns NULL if the file failed to parse, or if it contain zero keys.
Certificate*
load_keys(const char* filename, int* numKeys) {
    Certificate* out = NULL;
    *numKeys = 0;

    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        LOGE("opening %s: %s\n", filename, strerror(errno));
        goto exit;
    }

    {
        int i;
        bool done = false;
        while (!done) {
            ++*numKeys;
            out = (Certificate*)realloc(out, *numKeys * sizeof(Certificate));
            Certificate* cert = out + (*numKeys - 1);
            cert->public_key = (RSAPublicKey*)malloc(sizeof(RSAPublicKey));

            char start_char;
            if (fscanf(f, " %c", &start_char) != 1) goto exit;
            if (start_char == '{') {
                // a version 1 key has no version specifier.
                cert->public_key->exponent = 3;
                LOGI("line:[%d]cert->hash_len:%d\n", __LINE__, cert->hash_len);
                cert->hash_len = SHA_DIGEST_SIZE;
            } else if (start_char == 'v') {
                int version;
                if (fscanf(f, "%d {", &version) != 1) goto exit;
                switch (version) {
                    case 2:
                        cert->public_key->exponent = 65537;
                        LOGI("line:[%d]cert->hash_len:%d\n", __LINE__, cert->hash_len);
                        cert->hash_len = SHA_DIGEST_SIZE;
                        break;
                    case 3:
                        cert->public_key->exponent = 3;
                        LOGI("line:[%d]cert->hash_len:%d\n", __LINE__, cert->hash_len);
                        cert->hash_len = SHA256_DIGEST_SIZE;
                        break;
                    case 4:
                        cert->public_key->exponent = 65537;
                        LOGI("line:[%d]cert->hash_len:%d\n", __LINE__, cert->hash_len);
                        LOGI("cert->hash_len:%d\n", cert->hash_len);
                        cert->hash_len = SHA256_DIGEST_SIZE;
                        break;
                    default:
                        goto exit;
                }
            }
            LOGI("line:[%d]cert->hash_len:%d\n", __LINE__, cert->hash_len);
            RSAPublicKey* key = cert->public_key;
            if (fscanf(f, " %i , 0x%x , { %u",
                       &(key->len), &(key->n0inv), &(key->n[0])) != 3) {
                goto exit;
            }
            if (key->len != RSANUMWORDS) {
                LOGE("key length (%d) does not match expected size\n", key->len);
                goto exit;
            }
            for (i = 1; i < key->len; ++i) {
                if (fscanf(f, " , %u", &(key->n[i])) != 1) goto exit;
            }
            if (fscanf(f, " } , { %u", &(key->rr[0])) != 1) goto exit;
            for (i = 1; i < key->len; ++i) {
                if (fscanf(f, " , %u", &(key->rr[i])) != 1) goto exit;
            }
            fscanf(f, " } } ");

            // if the line ends in a comma, this file has more keys.
            switch (fgetc(f)) {
            case ',':
                // more keys to come.
                break;

            case EOF:
                done = true;
                break;

            default:
                LOGE("unexpected character between keys\n");
                goto exit;
            }

            LOGI("read key e=%d hash=%d\n", key->exponent, cert->hash_len);
        }
    }

    fclose(f);
    return out;

exit:
    if (f) fclose(f);
    free(out);
    *numKeys = 0;
    return NULL;
}
