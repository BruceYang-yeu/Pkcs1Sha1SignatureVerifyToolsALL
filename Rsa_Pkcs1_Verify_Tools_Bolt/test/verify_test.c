#include <unistd.h>
#include <errno.h>
#include <ctype.h>

#include "osdep.h"
#include "verifier.h"
#include "sha.h"

#define VERIFY_SUCCESS        1
#define VERIFY_FAILURE        0

static void usage() ;
static void Verify(char *argv);

#define MALLOC_EXPANT 4096

int main(int argc,  char * argv[])
 {
    int c;

    opterr = 0;
    while ((c = getopt(argc, argv, "hv:")) != -1)
    {
     switch (c)
     {
      case 'h':
        usage();
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
Verify(char *argv)
{
    const char *delim = " ";
    char *file = NULL, *KeyFile = NULL;
    unsigned int mDefSignaLen = 256;
    status_t ret=0;
    int numKeys = 0;
    unsigned int FileSize = 0, originalSize = 0;
    unsigned char * buffer = NULL, *originalBuffer = NULL;
    FILE *signatureFile = NULL;
    Certificate* mCertificate = NULL;
    
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
       
    PRO_START_PRINT
    //Extraction last 256 signatures
    signatureFile = fopen(file, "rb");
    fseek(signatureFile, 0, SEEK_END);
    FileSize = ftell(signatureFile);
    rewind(signatureFile);
    printf("You selected KeyFile is:%s\n", KeyFile);
    printf("Reading the %s document Len: %d to be Verify…\n", file, FileSize);
    if(FileSize < mDefSignaLen){
        fprintf(stderr, "%s is invaild signature file.\n", file);
        exit(-1);
    }
    mCertificate = load_keys((const char*)KeyFile, &numKeys);
    if (mCertificate == NULL) {
        LOGE("Failed to load keys\n");
        exit(-1);
    }
    printf("Load %d Key.\n", numKeys);
    buffer = (unsigned char *)malloc(sizeof(unsigned char) * FileSize + MALLOC_EXPANT);
    if (fread(buffer, 1, FileSize, signatureFile) != FileSize) {
        printf("fread File load error!\n");
        exit(-1);
    }
    fclose(signatureFile);
    printf("File read successfully, length: %d\n", FileSize);

    originalSize = FileSize - mDefSignaLen;
    printf("original File Size: %d\n", originalSize);
    originalBuffer = (unsigned char*)malloc(sizeof(char) * originalSize + MALLOC_EXPANT);
    memcpy(originalBuffer, buffer + mDefSignaLen, originalSize);
    //Verify
    printf("\033[1;32;40mVerifying signatures…\033[0m\n");      
    ret = Verify_SHA1((const RSAPublicKey*)mCertificate->public_key, originalBuffer, originalSize, buffer, mDefSignaLen);
    if (ret == VERIFY_SUCCESS) {
        printf("\033[1;34;40mVerifying signatures successful!\033[0m\n");
    } else {
        printf("\033[1;35;40mVerifying signatures failure!\033[0m\n");
    }
    free(mCertificate);
    free(originalBuffer);
    free(buffer);
    PRO_END_PRINT
}

static void usage() {
    fprintf(stderr, "%s\n"
        "Usage:%s\n"
        "  Options:\n"
        "   -h                           # Useage Information\n"
        "   -v \033[5;31;40m\"\033[0m\033[0;34;40mVerifyFile PubKeyFile\033[5;31;40m\"\033[0m\033[0m   # Verifying signatures\n"
        "\n A RSASSA_PKCS1#_SIGNED and RSASSA_PKCS1#_VERIFY implements.\n",    
        ME_TITLE, ME_NAME);
    exit(-1);
}