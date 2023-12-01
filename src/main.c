#include <stdio.h>
#include <stdlib.h>
#include "../include/tarsau.h"
#include <string.h>
#include <stdbool.h>

#define MAX_TOTAL_SIZE 200 * 1024 * 1024 // 200 MB
#define MAX_FILE_COUNT 32
void isAboveMaxSize(int total)
{
    if (total > MAX_TOTAL_SIZE)
    {
        printf("Above 200MB!. Aborting\n");
        exit(1);
    }
}
void isAboveMaxFileCount(int numOfFiles)
{
    if (numOfFiles > MAX_FILE_COUNT)
    {
        printf("Above 32 files! Aborting\n");
        exit(1);
    }
}
void checkArgs(int argc, char const *argv[])
{
    if (argc == 1)
    {
        printf("Bad usage. try app -a(extract) or -b(create)\n");
        exit(1);
    }
    if (strcmp(argv[1], "-b") == 0 && argc < 3)
    {
        printf("Create: app -b text_files -o archive_file_name.sau\n");
        exit(1);
    }
    if (strcmp(argv[1], "-a") == 0 && argc < 4)
    {
        printf("Extract: app -a archive_file_name.sau  directory_name\n");
        exit(1);
    }
}

int main(int argc, char const *argv[])
{
    checkArgs(argc, argv);
    char *inputFiles[MAX_FILE_COUNT];
    int j = 0;
    // Get all the input files
    int numOfFiles = 0;
    for (int i = 2; i < argc; i++)
    {
        if (strcmp(argv[i], "-o") == 0)
            break;
        else
        {
            if (checkFileExtension(argv[i]))
            {
                inputFiles[j] = argv[i];
                numOfFiles++;
            }
        }
        j++;
    }
    int total = calculateTotalFileSize(inputFiles, numOfFiles);
    isAboveMaxFileCount(numOfFiles);
    isAboveMaxSize(total);
    if (strcmp(argv[1], "-a") == 0)
    {
        const char *archiveFileName = argv[2];
        // extract
        if (checkArchiveExtension(archiveFileName))
        {
            checkArchiveFile(archiveFileName);
        }
    }
    else
    {
        // create
        const char *outputFileName = determineOutputFileName(argc, argv);
        create(outputFileName, inputFiles, numOfFiles);
    }
    return 0;
}
