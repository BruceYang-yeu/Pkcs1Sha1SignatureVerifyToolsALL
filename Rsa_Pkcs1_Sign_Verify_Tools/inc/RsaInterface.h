#ifndef RSA_INSPUR_INTERFACE_H
#define RSA_INSPUR_INTERFACE_H

#include <stdio.h>
#include <string>
#include <openssl/rsa.h>
#include <osdep.h>

using namespace std;

/**
 * SignVerifyInterface is the abstraction interface for the sign and verify a X509_SIG ASN1 object.
 * It inside PKCS#1 padded Rsa Encryption
 */
class SignVerifyInterface {
public:
    ~SignVerifyInterface(){}
    /**
     * check to see if the SignVerifyInterface interface has been initialized.
     */
    virtual status_t    initCheck() = 0;
    
	/**
     * Sign 
     */
    virtual status_t Sign(unsigned char *inBuffer,
					  unsigned long inBufferLen,
					  unsigned char **outSignature,
					  unsigned int *outSignatureLen )  = 0;
	/**
     * Verify
     */
    virtual status_t Verify(unsigned char *inData,
					     unsigned long inDataLen,
					     unsigned char *pSignature,
						 unsigned long pSigLen)  = 0;
	
    /**This method dumps the state */
    //virtual status_t dumpState(int fd, const Vector<String16>& args) = 0;
	
	static SignVerifyInterface* create();

protected:
    /** dump the state*/
    //virtual status_t dump(int fd, const Vector<String16>& args) = 0;

};

#endif // RSA_INSPUR_INTERFACE_H
