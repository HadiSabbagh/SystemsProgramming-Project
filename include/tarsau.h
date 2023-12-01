#ifndef TARSAU_H
#define TARSAU_H
#include <stdbool.h>
int calculateTotalFileSize(const char inputFiles[], int numOfFiles);
char *determineOutputFileName(int argc, char const *argv[]);
void checkArchiveFile(const char *filename);
bool checkFileExtension(const char *filename);
bool checkArchiveExtension(const char *filename);
void extract(const char *archiveFileName, const char *directoryName);
void create(const char *outputFileName, const char *inputFiles[], int numOfFiles);

#endif
