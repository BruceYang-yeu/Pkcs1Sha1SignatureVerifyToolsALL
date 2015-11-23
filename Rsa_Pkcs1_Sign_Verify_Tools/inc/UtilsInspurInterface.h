#ifndef RSA_UTILS_INSPUR_INTERFACE_H
#define RSA_UTILS_INSPUR_INTERFACE_H

#include <stdio.h>
#include <string>
#include <osdep.h>

struct MovelSuperTvHead{
	char Name[8];
	uint32_t Length;
	char Type;
    char staAttr;
	short dynAttr;
}__attribute__ ((aligned (8)));
typedef struct MovelSuperTvHead MovelSuperTvHead;


using namespace std;
 /**
 * RsaKeyInterface is the abstraction interface for RSA Struct.
 */
class InspurUtilsInterface {
public:
    virtual ~InspurUtilsInterface(){}
    virtual char* GetFileName(char* path) = 0;

    /** get Pfile size - eg. 3 */
    virtual uint32_t GetFileSize(FILE* pFile) = 0;

    /**
     * add a signature to given filename
     */
    virtual status_t  AddSignHead(char* fileName, 
                                  const unsigned char *signature, 
                                  unsigned int slen) = 0;
	/**
     * remove singnature from filename
     */
    virtual status_t  RemoveSigHead(char* fileName) = 0;
	/**
     * Add Nove SupperTvHead V3
     */
    virtual status_t    AddMovSuperTvHead(char* fileName, MovelSuperTvHead *mMovelSuperTvHead) = 0;
	
    /** dump the state*/
    //virtual status_t dump(int fd, const Vector<String16>& args) = 0;

};
#endif // RSA_UTILS_INSPUR_INTERFACE_H
