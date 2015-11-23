#ifndef __OSDEP_FILE__
#define __OSDEP_FILE__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#undef PUBLIC
#undef PUBLIC_DATA
#undef PRIVATE

#if ME_WIN_LIKE
    /*
        Use PUBLIC on function declarations and definitions (*.c and *.h). 
     */
    #define PUBLIC      __declspec(dllexport)
    #define PUBLIC_DATA __declspec(dllexport)
    #define PRIVATE     static
#else
    #define PUBLIC
    #define PUBLIC_DATA extern
    #define PRIVATE     static
#endif

#ifndef ME_TITLE
     #define ME_TITLE "version 1.0.0"
#endif
#ifndef ME_NAME
     #define ME_NAME "sigtool option"
#endif

#ifndef MAXINT
#if INT_MAX
    #define MAXINT      INT_MAX
#else
    #define MAXINT      0x7fffffff
#endif
#endif

#ifndef max
    #define max_ (a,b)  (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
    #define min_(a,b)  (((a) < (b)) ? (a) : (b))
#endif

#ifndef PRO_START_PRINT
	#define PRO_START_PRINT	fprintf(stdout, "\033[1;31;40mProgram Start...\033[0m\n" );
#endif
#ifndef PRO_END_PRINT	
	#define PRO_END_PRINT  fprintf(stdout, "\033[1;31;40mProgram End\033[0m\n" );
#endif

//#define DEBUG
#ifndef DEBUG
	#define DEBUG_HELP fprintf(stdout, "\033[1;31;40m[debug]Line: %d Function: %s\n\033[0m", __LINE__, __FUNCTION__);
#else
	#define DEBUG_HELP
#endif

#ifndef ANDROID

#define status_t int
#define BAD_VALUE -1
#define NO_ERROR  0

/* error codes */
#define BAD_DATA  1
#define GENERATOR_ERROR  2
#define KEY_GEN_ERROR 3
#define MEM_ALLOC_ERROR 4
#define STRING_SET_ERROR 5
#define BUF_SHORT 6

/* verification errors */
#define SELF_SIGN_OR_PATH 7
#define CERT_EXPIRED 8
#define CA_INVALID 9
#define SIGN_ERROR 10
#define VERIFY_ERROR 11
#define  OK 0
#define  SHA_DIGEST_LENGTH  20

#endif


typedef unsigned char uint8_t;
typedef unsigned int  uint32_t __attribute__ ((aligned (8)));
typedef long long int64_bit;

#endif /* defined(__OSDEP_FILE__) */
