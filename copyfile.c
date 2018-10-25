#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* sfnConcat(const char *pStr1, const char *pStr2);


int32_t main(int32_t argc, char* argv[])
{
    FILE *pSourceFile;
    FILE *pCopyFile;
    int dwCh;
    char* sCpFileName = sfnConcat(argv[1], "_copy");

    if(argc<=2)
    {
        
        pSourceFile = fopen(argv[1], "r");
        pCopyFile = fopen(sCpFileName, "w");

        while(1)
        {
            dwCh = fgetc(pSourceFile);

            if(!feof(pSourceFile))
            {
                fputc(dwCh, pCopyFile);
            }
            else
            {
                break;
            }
        }
    }
    fclose(pSourceFile);
    fclose(pCopyFile);
    free(sCpFileName);

    return 0;
}

char* sfnConcat(const char *pStr1, const char *pStr2)
{
    char *pCCStr = malloc(strlen(pStr1)+strlen(pStr2)+1); //+1 for null-end
    //Should check 4 errors
    strcpy(pCCStr, pStr1);
    strcat(pCCStr, pStr2);
    return pCCStr;
}

