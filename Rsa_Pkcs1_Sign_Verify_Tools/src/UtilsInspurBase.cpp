#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <UtilsInspurBase.h>

InspurUtilsBase::InspurUtilsBase():mSignedLen(256)
{
    
}

InspurUtilsBase::~InspurUtilsBase()
{
    
}

/**
* Extracts filename from the given path
* @param[in] path Path to extract filename from
* @return Extracted filename
*/
char* InspurUtilsBase::GetFileName(char* path)
{
    if(path == NULL){
        fprintf(stderr, "path is NULL\n");
        exit(-1);
    }
    char* fileName = strrchr(path, '/');

    if (fileName == NULL) {
        fileName = path;
    } else {
        fileName++;
    }

    return fileName;    
}

/**
* Calculates the given open file size
* @param[in] pFile pointer to file stream
* @return Given file size
*/
uint32_t InspurUtilsBase::GetFileSize(FILE* pFile)
{
    if(pFile != NULL){
        fseek(pFile, 0, SEEK_END);
        uint32_t fileSize = ftell(pFile);
        rewind(pFile);
    
        return fileSize;   
    } else {
        fprintf(stderr, "File Is NULL\n");
        exit(-1);
    }
}

/**
* Writes slen bytes at beginning of the given file
* @param[in] fileName
*/
status_t InspurUtilsBase::AddSignHead(char* fileName, 
                         const unsigned char *signature, 
                         unsigned int slen)
{
    const char *bakStr = ".bak";
    char tempArr[256] = {""};
    
    if(fileName == NULL|| signature == NULL || slen == 0) 
    {
        fprintf(stderr, "Argv Is Invalid\n");
        return BAD_VALUE;
    }
    strncpy(tempArr, fileName, strlen(fileName));
    FILE* pFile = fopen(fileName, "rb+");
    if (pFile == NULL) {
        fprintf(stderr, "Can't open file.\n");
        return BAD_VALUE;
    }


    char* bakFileName = strcat(tempArr, bakStr);
    printf("bakFileName:%s\n", bakFileName);
    FILE *pBakFile = fopen(bakFileName, "wb+");
    if (pBakFile == NULL) {
        fprintf(stderr, "Can't create backup file.\n");
        return BAD_VALUE;
    }

    char buffer[1024];
    size_t n;
    memset(buffer, 0, sizeof(buffer));
    while((n = fread(buffer, sizeof(char), sizeof(buffer), pFile)) > 0) {
        fwrite(buffer, sizeof(char), n, pBakFile);
    }

    printf("\033[1;32;40mAddSignHead...\033[0m\n");
    fseek(pBakFile, 0, SEEK_SET);
    fseek(pFile, 0, SEEK_SET);
    fwrite((const void*)signature, 1, slen, pFile);

    while((n = fread(buffer, sizeof(char), sizeof(buffer), pBakFile)) > 0) {
        fwrite(buffer, sizeof(char), n, pFile);
    }

    printf("Add SignatureHead Done.\n");

    fclose(pBakFile);
    fclose(pFile);
    unlink(bakFileName);

    return NO_ERROR;    
}

/**
* Removes first mSignedLen bytes from the given file.
* @param[in] fileName
*/
status_t InspurUtilsBase::RemoveSigHead(char* fileName)
{
    char tempArr[256] = {""};
   
    if(fileName == NULL){
        fprintf(stderr, "File Is NULL\n");
        exit(-1);
    }
    strncpy(tempArr, fileName, strlen(fileName));
    
    FILE* pFile = fopen(fileName, "rb+");
    if (pFile == NULL) {
        fprintf(stderr, "Can't open file.\n");
        return BAD_VALUE;
    }

    if (GetFileSize(pFile) < mSignedLen) {
        fprintf(stderr, "Invalid file.\n");
        return BAD_VALUE;
    }

    char* bakFileName = strcat(tempArr, ".bak");
    FILE *pBakFile = fopen(bakFileName, "wb+");
    if (pBakFile == NULL) {
        fprintf(stderr, "Can't create backup file.\n");
        return BAD_VALUE;
    }

    char buffer[1024];

    size_t n;
    while((n = fread(buffer, sizeof(char), sizeof(buffer), pFile)) > 0) {
        fwrite(buffer, sizeof(char), n, pBakFile);
    }

    printf("Remove signature Head...\n");

    fseek(pBakFile, mSignedLen, SEEK_SET);
    fseek(pFile, 0, SEEK_SET);

    while((n = fread(buffer, sizeof(char), sizeof(buffer), pBakFile)) > 0) {
        fwrite(buffer, sizeof(char), n, pFile);
    }

    printf("Remove Signature Head Done.\n");

    fclose(pBakFile);
    fclose(pFile);

    return NO_ERROR;    
}

status_t InspurUtilsBase::AddMovSuperTvHead(char* fileName, MovelSuperTvHead *mMovelSuperTvHead)
{
    const char *bakStr = ".oribak";
    const char *DataName = "Inspur";
    char tempArr[256] = {""};
    uint32_t mFileLen = 0;

    if(fileName == NULL|| mMovelSuperTvHead == NULL) 
    {
        fprintf(stderr, "Argv Is Invalid\n");
        return BAD_VALUE;
    }
    strncpy(tempArr, fileName, strlen(fileName));
    FILE* pFile = fopen(fileName, "rb+");
    if (pFile == NULL) {
        fprintf(stderr, "Can't open file.\n");
        return BAD_VALUE;
    }
    mFileLen = GetFileSize(pFile);
    printf("Before Add NovelSuperTvHead Filename:%s: Len:%d\n", fileName, mFileLen);
    printf("Novel SuperTv Head Len:%ld\n", sizeof(MovelSuperTvHead));
    memset(mMovelSuperTvHead, 0, sizeof(MovelSuperTvHead));
    strcpy(mMovelSuperTvHead->Name, DataName);
    mMovelSuperTvHead->Length = mFileLen;
    
    char* bakFileName = strcat(tempArr, bakStr);
    FILE *pBakFile = fopen(bakFileName, "wb+");
    if (pBakFile == NULL) {
        fprintf(stderr, "Can't create backup file.\n");
        return BAD_VALUE;
    }

    char buffer[1024];
    size_t n;
    memset(buffer, 0, sizeof(buffer));
    while((n = fread(buffer, sizeof(char), sizeof(buffer), pFile)) > 0) {
        fwrite(buffer, sizeof(char), n, pBakFile);
    }

    printf("\033[1;32;40mAdd MovSuperTv Head...\033[0m\n");
    fseek(pBakFile, 0, SEEK_SET);
    fseek(pFile, 0, SEEK_SET);
    fwrite((const void*)mMovelSuperTvHead, 1, sizeof(MovelSuperTvHead), pFile);

    while((n = fread(buffer, sizeof(char), sizeof(buffer), pBakFile)) > 0) {
        fwrite(buffer, sizeof(char), n, pFile);
    }

    printf("Add Movel SuperTv Head Done.\n");
    mFileLen = GetFileSize(pFile);
    printf("After Add NovelSuperTvHead Filename:%s: Len:%d\n", fileName, mFileLen);
    fclose(pBakFile);
    fclose(pFile);
    unlink(bakFileName);

    return NO_ERROR;    
}

