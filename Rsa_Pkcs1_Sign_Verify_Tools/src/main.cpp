#include <iostream>
#include <unistd.h>
#include <errno.h>
#include "osdep.h"

#include "UtilsInspurInterface.h"
#include "UtilsInspurBase.h"
#include "RsaInterface.h"
#include "RsaBase.h"

using namespace std;

static void usage() ;
static void AddNovelSupertvHeadV3(char *argv);
static void GenerateNewKey(int num, unsigned long e, string PubkeyFile, string PriKeyFile);
static void Signature(char *argv);
static void Verify(char *argv);

#define MALLOC_EXPANT 4096

int main(int argc,  char * argv[])
 {
    int c;

    opterr = 0;
    while ((c = getopt(argc, argv, "ga:s:v:")) != -1)
    {
     switch (c)
     {
      case 'a':
        AddNovelSupertvHeadV3(optarg);
        goto End;
        break;
      case 'g':
        GenerateNewKey(2048, 3, "publickey.pem", "privatekey.pem");
        goto End;
        break;
      case 's':
        Signature(optarg);
        goto End;
        break;
      case 'v':
        Verify(optarg);
        goto End;
        break;
      case '?':
      default:
        if (optopt == 'v')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (optopt == 's')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr,
                   "Unknown option character `\\x%x'.\n",
                   optopt);
        usage();
        exit(EXIT_FAILURE);      
    }
  }
  usage();
End:  
    return 0;
}

static void PUBLIC
AddNovelSupertvHeadV3(char *argv)
{
    MovelSuperTvHead *mMovelSuperTvHead = NULL;
    InspurUtilsBase *mInspurUtilsBase = NULL;
    status_t ret;
    
    PRO_START_PRINT
    if(NULL == argv){
        fprintf(stderr, "argv is NULL, invaild parament\n");
        exit(-1);
    }

    if(access(argv, F_OK) == -1) {   //F_OK(0) Test for existence
        fprintf(stderr, "%s:%s\n", argv, strerror(errno));
        exit(-1);
    }

    mMovelSuperTvHead = new MovelSuperTvHead;
    if(mMovelSuperTvHead == NULL){
        fprintf(stderr, "new error\n");
        exit(-1);
    }
    
    mInspurUtilsBase = new InspurUtilsBase();
    ret = mInspurUtilsBase->AddMovSuperTvHead(argv, mMovelSuperTvHead);
    if (ret == NO_ERROR)
    {
        fprintf(stdout, "Add MovSuperTvHead V3 successfully!\n");
    }else{
        fprintf(stdout, "Add MovelSuperTvHead V3 Failure!\n");
    }

    delete mMovelSuperTvHead;
    delete mInspurUtilsBase;
    PRO_END_PRINT
}

static void PUBLIC 
GenerateNewKey(int num, unsigned long e, string PubkeyFile, string PriKeyFile)
{
    RsaKeyBase *mRsaKeyBase;
    status_t ret;
    
    mRsaKeyBase = new RsaKeyBase(num, e);
    ret = mRsaKeyBase->generateNewKey(PubkeyFile, PriKeyFile);
    if(ret == NO_ERROR){
        cout<<PubkeyFile<<" And "<<PriKeyFile<<"Have been Saved."<<endl;
    }else{
        cout<<"generateNewKey Error!"<<endl;
    }
    
    delete mRsaKeyBase;
}
static void PUBLIC
Signature(char *argv)
{
    const char *delim = " ";
    char *file = NULL, *KeyFile = NULL;
    //MovelSuperTvHead  *mMovelSuperTvHead = NULL;
    status_t ret;
    RsaKeyBase *mRsaKeyBase = NULL;
    SignVerifyBase *mSignVerifyBase = NULL;
    InspurUtilsBase *mInspurUtilsBase = NULL;
    

    
    file = strtok(argv, delim);
    KeyFile = strtok(NULL, delim);
    if (NULL == file || KeyFile == NULL) usage();
    
    if(access(KeyFile, F_OK) == -1) {   //F_OK(0) Test for existence
        fprintf(stderr, "%s:%s\n", KeyFile, strerror(errno));
        exit(-1);
    }
    if(access(file, F_OK) == -1) {   //F_OK(0) Test for existence
        fprintf(stderr, "%s:%s\n", file, strerror(errno));
        exit(-1);
    }

    string PriKeyFile(KeyFile);
    string PubKeyFile = "";
    mRsaKeyBase = new RsaKeyBase(PubKeyFile, PriKeyFile);
    mSignVerifyBase = new SignVerifyBase();
    ret = mSignVerifyBase->set(mRsaKeyBase);
    if (ret != NO_ERROR){
        cout<<"mSignVerifyBase set Failure"<<endl;
        exit(-1);
    }
    mInspurUtilsBase = new InspurUtilsBase();
    PRO_START_PRINT
    printf("You selected KeyFile is:%s\n", KeyFile);
    printf("Reading the %s document to be signed…\n", file);
    //mMovelSuperTvHead = new MovelSuperTvHead;
    //rsa->AddMovSuperTvHead(file, mMovelSuperTvHead);
    FILE *originalFile = fopen(file, "rb");
    if (originalFile == NULL) {
        printf("File not exits!\n");
        exit(0);
    }
    fseek(originalFile, 0, SEEK_END);
    unsigned int originalSize = ftell(originalFile);
    rewind(originalFile);
    unsigned char *originalBuffer = (unsigned char *)malloc(sizeof(char) * originalSize +1024);
    if (originalBuffer == NULL) {
        printf("Memory Error!\n"); 
        exit(0);
    }
    
    if (fread(originalBuffer, 1, originalSize, originalFile) != originalSize) {
        printf("File load error!\n");
        exit(0);
    }
    fclose(originalFile);
    printf("File read successfully, length:%d\n", originalSize); 
    printf("\033[1;32;40mStart Signing…\033[0m\n");
    //sign
    
    unsigned int *signatureLength = 0;
    unsigned char *signature =NULL;

    signatureLength = new unsigned int;
    
     ret = mSignVerifyBase->Sign((unsigned char *)originalBuffer, originalSize, &signature, signatureLength);
     if (ret != NO_ERROR)
     {  printf("Signature Error!\n");
        exit(-1);
     } 
    printf("signatureLength:%d\n", *signatureLength);
    
#ifdef DEBUG
    for(int i = 0; i < *signatureLength; i++) 
    { 
        if (i%6==0) 
            printf("\n%08xH: ",i); 
            printf("%02x ", *signature[i]); 
    } 
    printf("\n");
#endif   
    
    //save signature
    ret = mInspurUtilsBase->AddSignHead(file, signature, *signatureLength);
    if (ret == NO_ERROR)
    {
        fprintf(stdout, "Add SignHead successfully!\n");
    }else{
        fprintf(stdout, "Add SignHead Failure!\n");
    } 

    free(originalBuffer);
    free(signature);
    delete signatureLength;
    delete mSignVerifyBase;
    delete mInspurUtilsBase;
    printf("Signature successful, signature length is% d, total length is% d," 
           "after signature file has been saved.\n", 
            *signatureLength, *signatureLength + originalSize);    
    PRO_END_PRINT
}

static void PUBLIC
Verify(char *argv)
{
    const char *delim = " ";
    char *file = NULL, *KeyFile = NULL;
    unsigned int mDefSignaLen = 256;
    status_t ret;
    RsaKeyBase *mRsaKeyBase = NULL;
    SignVerifyBase *mSignVerifyBase = NULL;
    
    file = strtok(argv, delim);
    KeyFile = strtok(NULL, delim);
    if (NULL == file || KeyFile == NULL) usage();
    if(access(KeyFile, F_OK) == -1) {   //F_OK(0) Test for existence
        fprintf(stderr, "%s:%s\n", KeyFile, strerror(errno));
        exit(-1);
    }
    if(access(file, F_OK) == -1) {   //F_OK(0) Test for existence
        fprintf(stderr, "%s:%s\n", file, strerror(errno));
        exit(-1);
    }
   
    string PubKeyFile(KeyFile);
    string PriKeyFile = "";
    mRsaKeyBase = new RsaKeyBase(PubKeyFile, PriKeyFile);
    mSignVerifyBase = new SignVerifyBase();
    ret = mSignVerifyBase->set(mRsaKeyBase);
    if (ret != NO_ERROR){
        cout<<"mSignVerifyBase set Failure"<<endl;
        exit(-1);
    }
    
    PRO_START_PRINT
    //Extraction last 256 signatures
    FILE *signatureFile = fopen(file, "rb");
    fseek(signatureFile, 0, SEEK_END);
    unsigned int FileSize = ftell(signatureFile);
    rewind(signatureFile);
    printf("You selected KeyFile is:%s\n", KeyFile);
    printf("Reading the %s document Len: %d to be Verify…\n", file, FileSize);
    if(FileSize < mDefSignaLen){
        fprintf(stderr, "%s is invaild signature file.\n", file);
        exit(-1);
    }

    unsigned char *buffer = (unsigned char *)malloc(sizeof(unsigned char) * FileSize + MALLOC_EXPANT);
    if (fread(buffer, 1, FileSize, signatureFile) != FileSize) {
        printf("fread File load error!\n");
        exit(-1);
    }
    fclose(signatureFile);
    printf("File read successfully, length: %d\n", FileSize);

    int originalSize = FileSize - mDefSignaLen;
    printf("original File Size: %d\n", originalSize);
    unsigned char *originalBuffer = (unsigned char*)malloc(sizeof(char) * originalSize + MALLOC_EXPANT);
    memcpy(originalBuffer, buffer + mDefSignaLen, originalSize);
    //Verify
    printf("\033[1;32;40mVerifying signatures…\033[0m\n");      
    ret = mSignVerifyBase->Verify(originalBuffer, originalSize, buffer, mDefSignaLen);
    if (ret == NO_ERROR) {
        printf("\033[1;34;40mVerifying signatures successful!\033[0m\n");
    } else {
        printf("\033[1;35;40mVerifying signatures failure!\033[0m\n");
    }
    free(originalBuffer);
    free(buffer);
    PRO_END_PRINT
}

static void usage() {
    fprintf(stderr, "%s\n"
        "Usage:%s\n"
        "  Options:\n"
        "   -a                           # Add Novel Super Tv Head V3\n"
        "   -g                           # Generates a pair of keys\n"
        "   -e                           # Cryptographic file\n"
        "   -d                           # Decrypt the encrypted file\n"
        "   -s \033[5;31;40m\"\033[0m\033[0;34;40mSignFile PriKeyFile\033[5;31;40m\"\033[0m\033[0m     # signature \n"
        "   -v \033[5;31;40m\"\033[0m\033[0;34;40mVerifyFile PubKeyFile\033[5;31;40m\"\033[0m\033[0m   # Verifying signatures\n"
        "\n A RSASSA_PKCS1#_SIGNED and RSASSA_PKCS1#_VERIFY implements.\n",    
        ME_TITLE, ME_NAME);
    exit(-1);
}