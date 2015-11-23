#ifndef UTILS_INSPUR_INTERFACE_BASE_H
#define UTILS_INSPUR_INTERFACE_BASE_H

#include <stdio.h>
#include <string>
#include <osdep.h>
#include <unistd.h>
#include <UtilsInspurInterface.h>


using namespace std;

/**
 * InspurUtilsBase is a convenient base class used for implementing the
 * InspurUtilsInterface interface.
 */
class InspurUtilsBase : public InspurUtilsInterface{
public:
    InspurUtilsBase();
    virtual    ~InspurUtilsBase();

    virtual char* GetFileName(char* path) ;
    virtual uint32_t GetFileSize(FILE* pFile);
    
    virtual status_t  AddSignHead(char* fileName, 
                         const unsigned char *signature, 
                         unsigned int slen);
    virtual status_t  RemoveSigHead(char* fileName);

    virtual status_t    AddMovSuperTvHead(char* fileName, MovelSuperTvHead *mMovelSuperTvHead);
	
    //virtual status_t dump(int fd, const Vector<String16>& args) = 0;
    
private:
    uint32_t mSignedLen;    

};

#endif // UTILS_INSPUR_INTERFACE_BASE_H
