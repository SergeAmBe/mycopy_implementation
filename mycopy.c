#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

char* ubafnConcat(const char *ubpStr1, const char *ubpStr2);

#define BUFF_SIZE 1024

int32_t main(int32_t argc, char* argv[])
{
    int32_t wSrcF;
    int32_t wDstF;
    int32_t wRdBuff;
    char *ubaBuff[BUFF_SIZE];

    if(argc != 2)
    {
        printf("\nUsage: ./mycopy.o source_file\n");
        exit(EXIT_FAILURE);
    }

    if(((wSrcF = open(argv[1],O_RDONLY)) == -1))
    {
        printf("\nError opening file %s errno = %d\n",argv[1],errno);
        exit(EXIT_FAILURE);
    }

    char *ubaCopyF = ubafnConcat(argv[1],"_copy");
    if(ubaCopyF == NULL)
    {
        exit(EXIT_FAILURE);
    }

    if((wDstF = open(ubaCopyF,O_WRONLY | O_CREAT | O_TRUNC,
                       S_IRUSR | S_IWUSR | S_IWGRP | S_IROTH | S_IWOTH)) == -1)
    {
        printf("\nError opening file %s errno = %d\n",ubaCopyF,errno);
        exit(EXIT_FAILURE);
    }

    while((wRdBuff = read(wSrcF,ubaBuff,BUFF_SIZE))> 0)
    {
        if(write(wDstF,ubaBuff,wRdBuff) != wRdBuff)
        {
            printf("\nError in writing data to %s\n",ubaCopyF);
        }
    }

    if(wRdBuff == -1)
    {
        printf("\nError in reading data from %s\n",argv[1]);
    }

    if(close(wSrcF) == -1)
    {
        printf("\nError: couldn't close %s file\n",argv[1]);
    }

    if(close(wDstF) == -1)
    {
        printf("\nError: couldn't close %s file\n",ubaCopyF);
    }

    free(ubaCopyF);

    exit(EXIT_SUCCESS);
    return 1;
}

char* ubafnConcat(const char *ubpStr1, const char *ubpStr2)
{
    char *ubpCCStr = malloc(strlen(ubpStr1)+strlen(ubpStr2)+1);
    
    if(ubpCCStr == NULL)
    {
        printf("\nError: Unable to allocate memory.\n");
	return ubpCCStr;
    }

    strcpy(ubpCCStr, ubpStr1);
    strcat(ubpCCStr, ubpStr2);
    return ubpCCStr;
}

