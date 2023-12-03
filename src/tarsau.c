#include "../include/tarsau.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

mode_t getPermissions(const char *filename)
{
    struct stat fileStat;
    if (stat(filename, &fileStat) == -1)
    {
        perror("Error getting file permissions");
        exit(1);
    }
    return fileStat.st_mode & 0777; // Extract the permission bits
}

off_t getFileSize(const char *filename)
{
    struct stat fileStat;
    if (stat(filename, &fileStat) == -1)
    {
        perror("Error getting file size");
        exit(1);
    }
    return fileStat.st_size;
}

int calculateTotalFileSize(char *inputFiles[], int numOfFiles)
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

    // Check if the "-o" option is provided and if there is a valid output file name
    if (argc >= 4 && strcmp(argv[argc - 2], "-o") == 0 && strlen(argv[argc - 1]) > 0)
    {
        return argv[argc - 1];
    }
    else
    {
        // If the output file name is not provided or is empty, use the default "a.sau"
        return "a.sau";
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
bool checkFileExtension(int fileArgcEnd, char const *argv[])
{
    for (int i = 2; i <= fileArgcEnd; i++)
    {
        const char *fileExtension;
        if ((strrchr(argv[i], '.')) != 0)
        {
            fileExtension = strrchr(argv[i], '.');
        }
        else
        {
            printf("Error: %s input file format is incompatible! \n", argv[i]);
            exit(1);
        }

        if (strcmp(fileExtension, ".txt") != 0)
        {
            printf("Error: %s input file format is incompatible! \n", argv[i]);
            exit(1);
        }
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
    char path[100];
    snprintf(path, sizeof(path), "%s", outputFileName);

    FILE *outputFile = fopen(path, "w");
    if (outputFile == NULL)
    {
        printf("Error opening output file");
        exit(1);
    }
    int totalSize = calculateTotalFileSize(inputFiles, numOfFiles);
    fprintf(outputFile, "%010d", totalSize);

    for (int i = 0; i < numOfFiles; i++)
    {
        fprintf(outputFile, "|%s,%o,%ld|", inputFiles[i], getPermissions(inputFiles[i]), getFileSize(inputFiles[i]));
    }

    for (int i = 0; i < numOfFiles; i++)
    {
        FILE *inputFile = fopen(inputFiles[i], "r");
        if (inputFile == NULL)
        {
            printf("Error opening input file: %s", inputFiles[i]);
            exit(1);
        }
        char buffer[50];
        size_t bytesRead;

        while ((bytesRead = fread(buffer, 1, sizeof(buffer), inputFile)) > 0)
        {
            fwrite(buffer, 1, bytesRead, outputFile);
        }
        fclose(inputFile);
    }
    fclose(outputFile);
}
