#ifndef KEY2_INSPUR_INTERFACE_H
#define KEY2_INSPUR_INTERFACE_H

#include <stdio.h>
#include <string>
#include <openssl/rsa.h>
#include <osdep.h>


using namespace std;

/**
 * RsaKeyInterface is the abstraction interface for RSA Struct.
 */
class RsaKeyInterface {
public:

    /** return RsaKey Bits - eg. 2048 */
    virtual int    rsaKeyBits() const = 0;

    /** returns RsaKey e - eg. 3 */
    virtual unsigned long    rsaKeyE() const = 0;

    /**
     * returns the pointer to the Publickey
     */
    virtual RSA*    getRsaPublicKey() const = 0;
	
	/**
     * returns the pointer to the Privatekey
     */
    virtual RSA*    getRsaPrivateKey() const = 0;
	
	/**
     * returns the Publickey file name
     */
    virtual string    getPubKeyFileName() const = 0;
	
	/**
     * returns the Privatekey file name
     */
    virtual string    getPriKeyFileName() const = 0;
	
	/**
     * Reload RsaKey By File Name
     */
    virtual status_t    keyReload()  = 0;
	
	/**
     * Generate RsaKey By File Name
     */
    virtual status_t generateNewKey(string publicKeyFile, string privateKeyFile) = 0;


    /** dump the state*/
    //virtual status_t dump(int fd, const Vector<String16>& args) = 0;

};
#endif // KEY_INSPUR_INTERFACE_H
