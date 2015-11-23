#include "KeyBase.h"
#include <iostream>
#include <openssl/rsa.h>
#include <openssl/pem.h>

//#define DEBUG
#ifndef DEBUG
    #define DEBUG_HELP fprintf(stdout, "\033[1;31;40m[debug]Line: %d Function: %s\n\033[0m", __LINE__, __FUNCTION__);
#else
    #define DEBUG_HELP
#endif

RsaKeyBase::RsaKeyBase(int keynumber, unsigned long  keye)
            :mRsa(NULL), mPrivateKey(NULL), mPublicKey(NULL), PriKeyFileName(NULL), PubKeyFileName(NULL),mKeyE(keye),mKeyNumber(keynumber)
{
    //mKeyE= keye;    
    //mKeyNumber = keynumber;
}

RsaKeyBase::RsaKeyBase(string publickeyfile, string privatekeyfile)
   :mRsa(NULL), mPrivateKey(NULL), mPublicKey(NULL), mKeyE(0),mKeyNumber(0)
{
    
        PriKeyFileName = privatekeyfile;
        if (!PriKeyFileName.empty()) {
            FILE *fp = fopen(PriKeyFileName.c_str(), "r");
            if (fp == NULL) {
                cout << "Private Key File Error!" << endl;
                return ;
            }
            mPrivateKey = PEM_read_RSAPrivateKey(fp, NULL, NULL, NULL);
            fclose(fp);
        }
  
        PubKeyFileName = publickeyfile;
        if (!PubKeyFileName.empty()) {
            FILE *fp = fopen(PubKeyFileName.c_str(), "r");
            if (fp == NULL) {
                cout << "Public Key File Error!" << endl;
                return ;
            }
            //rsa = PEM_read_RSA_PUBKEY(fp, NULL, NULL, NULL);
            mPublicKey = PEM_read_RSAPublicKey(fp, NULL, NULL, NULL);
            fclose(fp);
            return ;
        }
}

RsaKeyBase::~RsaKeyBase()
{
    if (mRsa != NULL)
        RSA_free(mRsa);
    if (mPrivateKey != NULL)
        RSA_free(mPrivateKey);
    if (mPublicKey != NULL)
        RSA_free(mPublicKey);    
}
status_t RsaKeyBase::set(string publickeyfile, string privatekeyfile)
{
    return NO_ERROR;
}
status_t RsaKeyBase::keyReload()
{
    if (!PriKeyFileName.empty()) {
        FILE *fp = fopen(PriKeyFileName.c_str(), "r");
        if (fp == NULL) {
            cout << "Private Key File Error!" << endl;
            return BAD_VALUE;
        }
        mPrivateKey = PEM_read_RSAPrivateKey(fp, NULL, NULL, NULL);
        fclose(fp);
    }
    if (!PubKeyFileName.empty()) {
        FILE *fp = fopen(PubKeyFileName.c_str(), "r");
        if (fp == NULL) {
            cout << "Public Key File Error!" << endl;
            return BAD_VALUE;
        }
        mPublicKey = PEM_read_RSAPublicKey(fp, NULL, NULL, NULL);
        fclose(fp);
    }
    
    return NO_ERROR;
}

status_t RsaKeyBase::generateNewKey(string publicKeyFile, string privateKeyFile)
{
    PriKeyFileName = privateKeyFile;
    PubKeyFileName = publicKeyFile;

    RSA *mRsa = RSA_generate_key(mKeyNumber, mKeyE, NULL, NULL);
    if (mRsa == NULL) {
        cout << "RSA_generate_key Error!" << endl;
        return BAD_VALUE;
    }
    
    if (!PriKeyFileName.empty())
    {
        BIO *priBio = BIO_new_file(PriKeyFileName.c_str(), "w");
        if (PEM_write_bio_RSAPrivateKey(priBio, mRsa, NULL, NULL, 0, NULL, NULL) <= 0) {
            cout << "Save to private key file error!" << endl;
            return BAD_VALUE;
        }
        BIO_free(priBio);
    } else {
        cout << "PriKeyFileName is empty!" << endl;
    }
    
    if (!PubKeyFileName.empty()) {
        BIO *pubBio = BIO_new_file(PubKeyFileName.c_str(), "w");
        if (PEM_write_bio_RSAPublicKey(pubBio, mRsa) <= 0) {
            cout << "Save to public key file error!" << endl;
            return BAD_VALUE;
        }
        BIO_free(pubBio);
    } else {
        cout << "PubKeyFileName is empty" <<endl;
    }

    this->mRsa = mRsa;
    mPrivateKey = RSAPrivateKey_dup(mRsa);
    mPublicKey = RSAPublicKey_dup(mRsa);    
    
    return NO_ERROR;
}

