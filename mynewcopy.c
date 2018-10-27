#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

#define BUFF_SIZE 1024
#define COPY_FILE_OFLAGS (O_WRONLY | O_CREAT | O_TRUNC)
#define COPY_FILE_PFLAGS (S_IRUSR | S_IWUSR | S_IWGRP | S_IROTH | S_IWOTH)

typedef enum CopyStates{
	CopyUsuageState = 0,
	SrcOpenState,
	CopyNameState,
	DestOpenState,
	BuffAllocState,
	CopyToFileState,
	CloseFreeState,
	EscpState
}ubCopyStates_t;

typedef struct CopyParams{
    int32_t wSrcFD;
	int32_t wDstFD;
	int32_t wRdBuff;
	char *bpBuff;
    char *bpCopyF;
	uint8_t ubCopyState;
}CopyParams_t;

char* ubpfnConcat(const char *ubpStr1, const char *ubpStr2);
void (*vpfnCopyStates[MAX_CP_STATES])(xCopyParams_t CopyParams) =
{
	void vfnCopyUsuage(CopyParams_t xCpParams);
	void vfnSrcOpen(CopyParams_t xCpParams);
	void vfnCopyName(CopyParams_t xCpParams);
	void vfnDestOpen(CopyParams_t xCpParams);
	void vfnBuffAlloc(CopyParams_t xCpParams);
	void vfnCopyToFile(CopyParams_t xCpParams);
	void vfnCloseFree(CopyParams_t xCpParams);
};

int32_t main(int32_t argc, char* argv[])
{
    CopyParams_t xCpParams;
	xCpParams.wSrcFD = argc;
	xCpParams.bpBuff = argv[1];
	xCpParams.ubCopyState = CopyUsuageState;
	
	do
	{
		(*vpfnCopyStates[xCpParams.ubCopyState])(xCpParams);
	}while(xCpParams.ubCopyState != EscpState)
	
	return 0;
}

void vfnCopyUsuage(xCopyParams_t xCpParams)
{
	if(xCpParams->wSrcFD != 2)
    {
        puts("\nUsage: ./mycopy.o source_file\n");
		xCpParams->ubCopyState = EscpState;
    }
	xCpParams->ubCopyState = SrcOpenState;
	return;
}

void vfnSrcOpen(xCopyParams_t xCpParams)
{
	xCpParams->wSrcFD = open(xCpParams->bpBuff,O_RDONLY);
    if(wSrcFD == -1)
    {
        puts("\nError opening file Source file.\n");
		xCpParams->ubCopyState = EscpState;
    }
	xCpParams->ubCopyState = CopyNameState;
	return;	
}

void vfnCopyName(CopyParams_t xCpParams)
{
    xCpParams->bpCopyF = ubpfnConcat(xCpParams->bpBuff,"_copy");
    if(xCpParams->bpCopyF == NULL)
    {
        puts("\nError allocating space for *_copy file name.\n");
		xCpParams->ubCopyState = EscpState;
    }
	xCpParams->ubCopyState = CopyNameState;
	return;	
}

void vfnDestOpen(CopyParams_t xCpParams)
{
    xCpParams->wDstFD = open(xCpParams->bpCopyF, COPY_FILE_OFLAGS, COPY_FILE_PFLAGS);
    if(xCpParams->wDstFD == -1)
    {
        printf("\nError opening file %s.\n",xCpParams->bpCopyF);
		xCpParams->DestOpenState = CloseFreeState;
    }	
	xCpParams->DestOpenState = CopyNameState;
	return;	
}

void vfnBuffAlloc(CopyParams_t xCpParams)
{
    xCpParams->ubpBuff = malloc(BUFF_SIZE);
    if(xCpParams->ubpBuff == NULL)
    {
        puts("\nError allocating space for writing buffer.\n");
		xCpParams->DestOpenState = CloseFreeState;
    }
	xCpParams->DestOpenState = CopyToFileState;
	return;	
}

void vfnCopyToFile(CopyParams_t xCpParams)
{
	while((xCpParams->wRdBuff = read(xCpParams->wSrcFD,xCpParams->ubpBuff,BUFF_SIZE))> 0)
    {
        if(write(xCpParams->wDstFD,xCpParams->ubpBuff,xCpParams->wRdBuff) != xCpParams->wRdBuff)
        {
            puts("\nError in writing data to *_copy file.\n");
        }
    }
	xCpParams->DestOpenState = CloseFreeState;
	return;	
}

void vfnCloseFree(CopyParams_t xCpParams)
{
    if(close(xCpParams->wSrcFD) == -1)
    {
        puts("\nError: couldn't close source file.\n");
    }
    if(close(xCpParams->wDstFD) == -1)
    {
        puts("\nError: couldn't close *_copy file.\n");
    }
    free(xCpParams->ubpCopyF);
    free(xCpParams->ubpBuff);
	xCpParams->DestOpenState = EscpState;
	return;
}

char* ubpfnConcat(const char *ubpStr1, const char *ubpStr2)
{
    char *ubpCCStr = malloc(strlen(ubpStr1)+strlen(ubpStr2)+1);

    strcpy(ubpCCStr, ubpStr1);
    strcat(ubpCCStr, ubpStr2);
    return ubpCCStr;
}