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
    uint32_t uwSrcF;
    uint32_t uwDstF;
    uint32_t uwRdBuff;
    char *ubaBuff[BUFF_SIZE];

    if(argc != 2)
    {
        printf("\nUsage: ./mycopy.o source_file\n");
        exit(EXIT_FAILURE);
    }

    if(((uwSrcF = open(argv[1],O_RDONLY)) == -1))
    {
        printf("\nError opening file %s errno = %d\n",argv[1],errno);
        exit(EXIT_FAILURE);
    }

    char *ubaCopyF = ubafnConcat(argv[1],"_copy");
    if(ubaCopyF == NULL)
    {
        exit(EXIT_FAILURE);
    }

    if((uwDstF = open(ubaCopyF,O_WRONLY | O_CREAT | O_TRUNC,
                       S_IRUSR | S_IWUSR | S_IWGRP | S_IROTH | S_IWOTH)) == -1)
    {
        printf("\nError opening file %s errno = %d\n",ubaCopyF,errno);
        exit(EXIT_FAILURE);
    }

    while((uwRdBuff = read(uwSrcF,ubaBuff,BUFF_SIZE))> 0)
    {
        if(write(uwDstF,ubaBuff,uwRdBuff) != uwRdBuff)
        {
            printf("\nError in writing data to %s\n",ubaCopyF);
        }
    }

    if(uwRdBuff == -1)
    {
        printf("\nError in reading data from %s\n",argv[1]);
    }

    if(close(uwSrcF) == -1)
    {
        printf("\nError: couldn't close %s file\n",argv[1]);
    }

    if(close(uwDstF) == -1)
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

