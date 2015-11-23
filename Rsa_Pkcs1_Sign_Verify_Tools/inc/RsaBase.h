#ifndef RSA_INSPUR_INTERFACE_BASE_H
#define RSA_INSPUR_INTERFACE_BASE_H

#include <stdio.h>
#include <string>
#include <RsaInterface.h>
#include <osdep.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <iostream>
#include <KeyInterface.h>
#include <KeyBase.h>

using namespace std;

class RsaKeyBase;
/**
 * SignVerifyBase is a convenient base class used for implementing the
 * SignVerifyInterface interface.
 */
class SignVerifyBase : public SignVerifyInterface
{
public:
    SignVerifyBase();
    virtual             ~SignVerifyBase();
    virtual status_t    set(RsaKeyBase *RsaKey);
    virtual status_t    initCheck();
   
    virtual status_t Sign(unsigned char *inBuffer,
					  unsigned long inBufferLen,
					  unsigned char **outSignature,
					  unsigned int *outSignatureLen );				  
    virtual status_t Verify(unsigned char *inData,
					     unsigned long inDataLen,
					     unsigned char *pSignature,
						 unsigned long pSigLen);
						 
    /**This method dumps the state */
    //virtual status_t dumpState(int fd, const Vector<String16>& args) = 0;
	
	static SignVerifyInterface* create();
	
protected:
    /** dump the state*/
    //virtual status_t dump(int fd, const Vector<String16>& args) = 0
	
private:
	RsaKeyBase *mRsaKey;
	uint32_t mSignedLen;
};

#endif  //RSA_INSPUR_INTERFACE_BASE_H