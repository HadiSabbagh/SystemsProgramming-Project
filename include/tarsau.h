#ifndef TARSAU_H
#define TARSAU_H
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>

mode_t getFilePermissions(const char *filename);                                                        // Get file permissions using filename
off_t getFileSize(const char *filename);                                                                // Get file size using filename
int getTotalFileSize(char *inputFiles[], int numOfFiles);                                               // Calculate total file size to check for MAX_TOTAL_SIZE (main.c)
char *determineOutputFileName(int argc, char const *argv[]);                                            // Determine whether to use a user-given parameter as the file name or use the default (a.sau)
void checkArchiveFile(const char *filename);                                                            // Tries to open the archive file.
bool checkFileExtension(const char *filename);                                                          // Checks if the file is an ASCII text file.
bool checkArchiveExtension(const char *filename);                                                       // Checks archive file's extension(must be .sau).
void checkForCurrentDirectory(const char *directory);                                                   // Checks whether the user has supplied the executable's directory as a directory for extraction. (Not allowed)
bool checkDirectory(const char *directory);                                                             // Checks whether a directory exists or not.
void createDirectory(const char *directory);                                                            // Creates a directory. (Dependent on checkForCurrentDirectory & checkDirectory)
char *getArchivedText(const char *archiveFileName);                                                     // Gets only the text part of the archive file.
void readAndTokenize(const char *archiveFileName, const char *directoryName);                           // Reads the archive file, tokenizes key words and calls writeExtract.
void extractFile(char *filename, int permissions, int filesize, char *text, const char *directoryName); // Creates a file at the directory, writes the original file's text, and sets file permissions.
int getMetaDataLength(const char *inputFiles[], int numOfFiles);                                        // Returns the archive file's metadata length.
void createArchive(const char *outputFileName, const char *inputFiles[], int numOfFiles);               // Creates the archive file based on the files supplied.
#endif
