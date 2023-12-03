#ifndef TARSAU_H
#define TARSAU_H
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>

mode_t getPermissions(const char *filename);
off_t getFileSize(const char *filename);
int calculateTotalFileSize(char *inputFiles[], int numOfFiles);
char *determineOutputFileName(int argc, char const *argv[]);
void checkArchiveFile(const char *filename);
bool checkFileExtension(int fileArgcEnd, char const *argv[]);
bool checkArchiveExtension(const char *filename);
void extract(const char *archiveFileName, const char *directoryName);
void create(const char *outputFileName, const char *inputFiles[], int numOfFiles);

#endif
