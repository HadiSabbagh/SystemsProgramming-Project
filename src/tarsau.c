#include "../include/tarsau.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <limits.h>

mode_t getFilePermissions(const char *filename)
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
int getTotalFileSize(char *inputFiles[], int numOfFiles)
{
    int size = 0;
    for (int i = 0; i < numOfFiles; i++)
    {
        /* // opening the file in read mode and calculate size
        FILE *fp = fopen(inputFiles[i], "r");
        if (fp == NULL)
        {
            printf("File Not Found! %s\n", inputFiles[i]);
            exit(1);
        }
        fseek(fp, 0L, SEEK_END);
        long int res = ftell(fp);
        size += res;
        fclose(fp); */
        size = getFileSize(inputFiles[i]);
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
FILE *getOpenFile(const char *filename, const char *mode)
{
    FILE *fp = fopen(filename, mode);
    if (fp == NULL)
    {
        printf("Error opening file %s with mode %s \n", filename, mode);
        exit(1);
    }
    return fp;
}
bool checkFileExtension(const char *filename)
{
    int character;
    FILE *file = getOpenFile(filename, "r");
    while ((character = fgetc(file)) != EOF) // Check if each character in the file is an ASCII character
    {
        if (character < 0 || character > 127) // Outside a-z and A-Z.
        {
            fclose(file);
            return false;
        }
    }
    fclose(file);
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
void createDirectory(const char *directory)
{
    struct stat st = {0};

    if (stat(directory, &st) == -1)
    {
        mkdir(directory, 0700);
    }
}
void checkForCurrentDirectory(const char *directory)
{
    char cwd[100]; // Max path length to check for
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        // If directory given is the current directory, abort.
        if (strcmp(cwd, directory) == 0)
        {
            printf("Extraction directory cannot be the same as the current working directory. Aborting...\n");
            exit(1);
        }
    }
}
bool checkDirectory(const char *directory)
{
    DIR *dir = opendir(directory);
    if (dir)
    {
        /* Directory exists*/
        closedir(dir);
        return true;
    }
    else
    {
        return false;
    }
}
void extractFile(char *filename, int permissions, int filesize, char *text, const char *directoryName)
{
    // Write File Content
    char *path = malloc(strlen(directoryName) + strlen(filename) + 3);
    if (path == NULL)
    {
        fprintf(stderr, "Memory allocation failure\n");
        exit(1);
    }

    snprintf(path, strlen(directoryName) + strlen(filename) + 2, "%s/%s", directoryName, filename);
    // printf("path: %s\n \n", path);

    FILE *inputFile = getOpenFile(path, "w");
    static int i = 0;
    int endOfSequence = i + filesize;
    while (i < endOfSequence && text != NULL && i < strlen(text))
    {
        int ascii = text[i];
        char txt = ascii;
        fwrite(&txt, 1, 1, inputFile);
        i++;
    }
    // Convert int permissions to string.
    char *permissionsString = malloc(sizeof(char) * 8);
    if (permissionsString == NULL)
    {
        printf("Memory allocation for permission string failed \n");
        exit(1);
    }
    sprintf(permissionsString, "0%d", permissions);

    // Convert permissionString to mode_t
    mode_t mode = (mode_t)(strtoul(permissionsString, NULL, 8));

    // Set permissions and free permissionsString;
    chmod(path, mode);
    free(permissionsString);
    free(path);
    fclose(inputFile);
    printf("%s , ", filename);
}
int getMetaDataLength(const char *inputFiles[], int numOfFiles)
{
    int byteSize = 0;
    int amount_of_pipes = numOfFiles * 2 - numOfFiles + 1;
    for (int i = 0; i < numOfFiles; i++)
    {
        byteSize += snprintf(NULL, 0, "%s,%o,%ld", inputFiles[i], getFilePermissions(inputFiles[i]), getFileSize(inputFiles[i]));
    }

    return byteSize + amount_of_pipes;
}
char *getArchivedText(const char *archiveFileName)
{

    FILE *outputFile = getOpenFile(archiveFileName, "r");
    if (outputFile == NULL)
    {
        printf("Error opening achive file");
        exit(1);
    }

    int totalSize = getFileSize(archiveFileName);
    char line[totalSize];
    char *filename;
    int permissions;
    int filesize;
    char *text = malloc(getFileSize(archiveFileName));
    if (text == NULL)
    {
        printf("No text section was found\n");
        exit(1);
    }
    while (fgets(line, totalSize + 1, outputFile) != NULL)
    {
        char *strtok_res = strtok(line, "|");
        int sectionSize = atoi(strtok_res);
        //  printf("%d section size\n", sectionSize);
        int sizeOfContent = 2; // pipe char for each file description
        while (strtok_res != NULL && sectionSize > 0)
        {

            // Extracting filename
            filename = strtok(NULL, ",");
            if (filename == NULL)
            {
                break;
            }
            //  printf("Filename: %s\n", filename);
            sizeOfContent += sizeof(strtok_res) / sizeof(char);
            // Extracting permissions
            strtok_res = strtok(NULL, ",");
            if (strtok_res == NULL)
            {
                break;
            }
            permissions = atoi(strtok_res);
            // printf("Permissions: %d\n", permissions);

            sizeOfContent += sizeof(strtok_res) / sizeof(int);
            // Extracting filesize
            strtok_res = strtok(NULL, "|");
            if (strtok_res == NULL)
            {
                break;
            }
            filesize = atoi(strtok_res);
            // printf("Filesize: %d\n", filesize);

            sizeOfContent += sizeof(strtok_res) / sizeof(int);

            // printf("size of content: %d\n", sizeOfContent);

            // printf("Calling writeExtract \n");
            sectionSize -= sizeOfContent;
            // printf("%d section size\n", sectionSize);
            sizeOfContent = 0;
            strtok_res = strtok(NULL, "|");
        }
        // printf("Text: %s\n", strtok_res);
        return strtok_res;
    }
}
void readAndTokenize(const char *archiveFileName, const char *directoryName)
{

    FILE *outputFile = getOpenFile(archiveFileName, "r");

    int totalSize = getFileSize(archiveFileName);
    char line[totalSize];
    char *filename;
    int permissions;
    int filesize;
    char *text = malloc(getFileSize(archiveFileName));
    strcpy(text, getArchivedText(archiveFileName));
    while (fgets(line, totalSize + 1, outputFile) != NULL)
    {
        char *strtok_res = strtok(line, "|");
        int sectionSize = atoi(strtok_res);
        int sizeOfContent = 2; // pipe char for each file description
        while (strtok_res != NULL && sectionSize > 0)
        {
            // Extracting filename
            filename = strtok(NULL, ",");
            if (filename == NULL)
            {
                break;
            }
            // printf("Filename: %s\n", filename);
            sizeOfContent += sizeof(strtok_res) / sizeof(char);
            // Extracting permissions
            strtok_res = strtok(NULL, ",");
            if (strtok_res == NULL)
            {
                break;
            }
            permissions = atoi(strtok_res);

            sizeOfContent += sizeof(strtok_res) / sizeof(int);
            // Extracting filesize
            strtok_res = strtok(NULL, "|");
            if (strtok_res == NULL)
            {
                break;
            }
            filesize = atoi(strtok_res);
            sizeOfContent += sizeof(strtok_res) / sizeof(int);
            sectionSize -= sizeOfContent;
            sizeOfContent = 0;

            extractFile(filename, permissions, filesize, text, directoryName);
        }
    }
}
void createArchive(const char *outputFileName, const char *inputFiles[], int numOfFiles)
{

    FILE *outputFile = getOpenFile(outputFileName, "w");

    // Get byte size of the first section
    int byteSize = getMetaDataLength(inputFiles, numOfFiles);

    // Write File Information (Size of first section)
    fprintf(outputFile, "%010d", byteSize);
    fprintf(outputFile, "|");
    // Write File Information (Name of file, permissions of file, size of file)
    for (int i = 0; i < numOfFiles; i++)
    {
        fprintf(outputFile, "%s,%o,%ld|", inputFiles[i], getFilePermissions(inputFiles[i]), getFileSize(inputFiles[i]));
    }
    // fprintf(outputFile, "|");

    // Write File Content
    for (int i = 0; i < numOfFiles; i++)
    {
        FILE *inputFile = getOpenFile(inputFiles[i], "r");
        char buffer[50];
        size_t bytesRead;

        while ((bytesRead = fread(buffer, 1, sizeof(buffer), inputFile)) > 0)
        {
            fwrite(buffer, 1, bytesRead, outputFile);
        }
        fclose(inputFile);
    }
    fclose(outputFile);
    printf("The files have been merged successfully.\n");
}
