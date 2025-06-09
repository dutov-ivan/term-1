#ifndef ACTION_UTILS_H
#define ACTION_UTILS_H
#include "../common/common.h"
#include <stddef.h>

// GENERAL
int handleErrorFileNotSpecified();
int skipFileHeader(FileContext *context);
int writeSignature(FileContext *context);

void showCurrentSortMethod();
int handleBadFileHeader(FileContext *context);

FileContext *createTempFile();

int handleFlushing(FileContext *context);
int truncateFileFromCurrentPosition(FileContext *context);

int handleFileIfEmpty(FileContext *context);
int handleFileIfNotExist(FileContext *context);

char *getValidFilename();
bool fileExists(const char *filename);

// REGIONS
int readRegion(Region *region, FileContext *context);
Region *initRegion();
void freeRegion(Region *region);
void printRegion(Region *region, size_t regionNumber);

Region *getRegionFromUser();
int writeRegion(Region *region, FileContext *context);
int editRegion(Region *region, size_t regionNumber);
int copyRegionsBetweenFiles(FileContext *source, FileContext *destination);
Region *readRegionAt(size_t recordNumber, FileContext *context);

Region **readAllRegions(size_t *regionCount, FileContext *context);
#endif
