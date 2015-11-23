#ifndef KEY1_INSPUR_INTERFACE_BASE_H
#define KEY1_INSPUR_INTERFACE_BASE_H

#include <stdio.h>
#include <string>
#include <osdep.h>
#include <KeyInterface.h>

using namespace std;

 /**
  * RsaKeyInterface is a convenient base class used for implementing the RsaKeyInterface 
  */

  class RsaKeyBase:RsaKeyInterface{
public:
    RsaKeyBase(int keynumber, unsigned long  keye);
    RsaKeyBase(string publickeyfile, string privatekeyfile);
    virtual    ~RsaKeyBase();
	virtual status_t    set(string publickeyfile, string privatekeyfile);
	virtual int    rsaKeyBits() const {return mKeyNumber;}
	virtual unsigned long    rsaKeyE() const {return mKeyE;}
	virtual RSA*    getRsaPublicKey() const {return mPublicKey;}
	virtual RSA*    getRsaPrivateKey() const {return mPrivateKey;}
	virtual string    getPubKeyFileName() const {return PubKeyFileName;}
	virtual string    getPriKeyFileName() const {return PriKeyFileName;}
	virtual status_t    keyReload() ;
	virtual status_t generateNewKey(string publicKeyFile, string privateKeyFile);
	//virtual status_t dump(int fd, const Vector<String16>& args) = 0;
	
private:
	RSA *mRsa;
	RSA *mPrivateKey;
	RSA *mPublicKey;
	string PriKeyFileName;
	string PubKeyFileName;
	unsigned long mKeyE; 	//key_e default: RSA_F4(65537)
	int mKeyNumber;
};
#endif  //KEY_INSPUR_INTERFACE_BASE_H