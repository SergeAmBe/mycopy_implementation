#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

char* ubpfnConcat(const char *ubpStr1, const char *ubpStr2);

#define BUFF_SIZE 1024
#define COPY_FILE_OPEN (O_WRONLY | O_CREAT | O_TRUNC)
#define COPY_FILE_PERMISIONS (S_IRUSR | S_IWUSR | S_IWGRP | S_IROTH | S_IWOTH)

int32_t main(int32_t argc, char* argv[])
{
    int32_t wSrcF;
    int32_t wDstF;
    int32_t wRdBuff;
    char *ubpBuff;
    char *ubpCopyF;

    if(argc == 2)
    {
        wSrcF = open(argv[1],O_RDONLY);
        if(wSrcF > -1)
        {
            ubpCopyF = ubpfnConcat(argv[1],"_copy");
            if(ubpCopyF != NULL)
            {
                wDstF = open(ubpCopyF, COPY_FILE_OPEN, COPY_FILE_PERMISIONS);
                if(wDstF > -1)
                {
                    ubpBuff = malloc(BUFF_SIZE);
                    if(ubpBuff != NULL)
                    {
                        while((wRdBuff = read(wSrcF,ubpBuff,BUFF_SIZE)) > 0)
                        {
                            if(write(wDstF,ubpBuff,wRdBuff) != wRdBuff)
                            {
                                printf("\nError in writing data to %s\n",ubpCopyF);
                            }
                        }
                    }
                    else{printf("\nError allocating space for writing buffer.\n");}
                }
                else{printf("\nError opening file %s.\n",ubpCopyF);}
            }
            else{printf("\nError allocating space for *_copy file name.\n");}
        }
        else{printf("\nError opening file %s.\n",argv[1]);}
    }
    else{printf("\nUsage: ./mycopy.o source_file\n");}

    if(close(wSrcF) == -1)
    {
        printf("\nError: couldn't close %s file\n",argv[1]);
    }

    if(close(wDstF) == -1)
    {
        printf("\nError: couldn't close %s file\n",ubpCopyF);
    }

    free(ubpCopyF);
    free(ubpBuff);

    return 1;
}

char* ubpfnConcat(const char *ubpStr1, const char *ubpStr2)
{
    char *ubpCCStr = malloc(strlen(ubpStr1)+strlen(ubpStr2)+1);

    strcpy(ubpCCStr, ubpStr1);
    strcat(ubpCCStr, ubpStr2);
    return ubpCCStr;
}

