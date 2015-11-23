#include <RsaBase.h>

//#define DEBUG
#ifndef DEBUG
    #define DEBUG_HELP fprintf(stdout, "\033[1;31;40m[debug]Line: %d Function: %s\n\033[0m", __LINE__, __FUNCTION__);
#else
    #define DEBUG_HELP
#endif

SignVerifyBase::SignVerifyBase():mSignedLen(256)
{
   
}

SignVerifyBase::~SignVerifyBase()
{
    if (mRsaKey){
	    delete mRsaKey;
    }	 
}

status_t SignVerifyBase::set(RsaKeyBase *RsaKey)
{
    if (RsaKey == NULL){
        fprintf(stderr, "RsaKey is NULL\n");
        exit(-1);
    }
    mRsaKey = RsaKey;
	return NO_ERROR; 
}

status_t SignVerifyBase::initCheck()
{
    if(mSignedLen != 256 || mRsaKey == NULL){
        fprintf(stderr, "Init Check Failure At %s[%d]", __FUNCTION__, __LINE__);
        return BAD_VALUE;
    }
    return NO_ERROR;
}

status_t SignVerifyBase::Sign(unsigned char *inBuffer, unsigned long inBufferLen,
			unsigned char **outSignature,
			unsigned int *outSignatureLen )
{
    RSA *keyLocal = NULL;                        /* OpenSSL RSA key structure */
    int cypherLen;                               /* RSA key length */
    unsigned char SHA1digest[SHA_DIGEST_LENGTH]; /* holds digests */
    int status;                                  /* status code for RSA_sign function, returned value */

   /* allocate RSA key */
   //keyLocal = RSA_new();

   /* put key modulos and private exponent in big number repr in newly allocated RSA key */
   //keyLocal->n = BN_bin2bn( key->privateKey->mod, key->privateKey->modBufLen, NULL );
   //keyLocal->d = BN_bin2bn( key->privateKey->exp, key->privateKey->expBufLen, NULL );
   mRsaKey->keyReload();   
   keyLocal = mRsaKey->getRsaPrivateKey();
   if(keyLocal == NULL){
       cout <<"Key Invaild"<<endl;
       exit(-1);
   }
   /* key length */
   cypherLen = RSA_size( keyLocal );
   printf("cypherLen:%d\n", cypherLen);
   DEBUG_HELP
   
   SHA1(inBuffer, inBufferLen, SHA1digest );
   *outSignature = (unsigned char*)malloc( sizeof(char) * cypherLen );
   
    /* sign the digest */
    status = RSA_sign( NID_sha1, SHA1digest, SHA_DIGEST_LENGTH, *outSignature, outSignatureLen, keyLocal );
    /* check for success */
    if ( status ) {
       RSA_free( keyLocal );
       return NO_ERROR;
    }else {
       RSA_free( keyLocal );
       return SIGN_ERROR;
    }
   return NO_ERROR;                
}

status_t SignVerifyBase::Verify(unsigned char *inData, unsigned long inDataLen,
              unsigned char *pSignature,
              unsigned long pSigLen)
{
    RSA *keyLocal;                               /* OpenSSL RSA key structure */
    int cypherLen;                               /* RSA key length */
    unsigned char SHA1digest[SHA_DIGEST_LENGTH]; /* holds digests */
    int status;                                  /* status code for RSA_sign function, returned value */
    
    mRsaKey->keyReload();    
    keyLocal = mRsaKey->getRsaPublicKey();
    if(keyLocal == NULL){
       cout <<"Key Invaild"<<endl;
       exit(-1);
   }
    /* key length */
    cypherLen = RSA_size( keyLocal );
    printf("cypherLen:%d\n", cypherLen);

    /* get the digest of message */
    SHA1( inData, inDataLen, SHA1digest );
    /* verify the digest against the signature */
    status = RSA_verify( NID_sha1, SHA1digest, SHA_DIGEST_LENGTH, pSignature, cypherLen, keyLocal );
    printf("RSA_verify Return:%d\n", status);
    /* check for errror and return */
    if ( status ){
         RSA_free( keyLocal );
         return NO_ERROR;
     } else {
         RSA_free( keyLocal );
         return VERIFY_ERROR;
      }    
}

