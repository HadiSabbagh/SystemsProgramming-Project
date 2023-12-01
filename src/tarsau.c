#include "../include/tarsau.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
int calculateTotalFileSize(const char inputFiles[], int numOfFiles)
{
    int size = 0;
    for (int i = 0; i < numOfFiles; i++)
    {
        // opening the file in read mode and calculate size
        FILE *fp = fopen(inputFiles[i], "r");
        if (fp == NULL)
        {
            printf("File Not Found! %s\n", inputFiles[i]);
            exit(1);
        }
        fseek(fp, 0L, SEEK_END);
        long int res = ftell(fp);
        size += res;
        fclose(fp);
    }
    return size;
}
char *determineOutputFileName(int argc, char const *argv[])
{
    char *outputFileName = NULL;
    // Check if the "-o" option is provided and if there is a valid output file name
    if (argc >= 4 && strcmp(argv[argc - 2], "-o") == 0 && strlen(argv[argc - 1]) > 0)
    {
        outputFileName = strdup(argv[argc - 1]);
        return outputFileName;
    }
    else
    {
        // If the output file name is not provided or is empty, use the default "a.sau"
        outputFileName = strdup("a.sau");
        return outputFileName;
    }
}
void checkArchiveFile(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Archive file is inappropriate or corrupt!\n");
        exit(1);
    }
}
bool checkFileExtension(const char *filename)
{
    const char *fileExtension = strrchr(filename, '.');
    if (strcmp(fileExtension, ".txt") != 0)
    {
        printf("Error: %s input file format is incompatible! \n", filename);
        exit(1);
    }
    return true;
}
bool checkArchiveExtension(const char *filename)
{
    const char *fileExtension = strrchr(filename, '.');
    if (strcmp(fileExtension, ".sau") != 0)
    {
        printf("Error: %s archive file format is incompatible! \n", filename);
        exit(1);
    }
    return true;
}
void extract(const char *archiveFileName, const char *directoryName)
{
    // TODO: Implement extraction logic
}

void create(const char *outputFileName, const char *inputFiles[], int numOfFiles)
{
    // TODO: Implement creation logic
}
