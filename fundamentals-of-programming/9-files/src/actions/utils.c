#include "utils.h"
#include "../common/common.h"
#include "../io/io.h"
#include "../io/utils.h"
#include "actions.h"
#include <errno.h>
#include <linux/limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static const char *filenameReject = "<>:\"/\\|?*";
static const char *disallowedNames[] = {".", ".."};

bool fileExists(const char *filename) {
  int result = access(filename, F_OK);
  if (errno == ENOENT ||
      errno == 0) { // if there's error related not to file non-existence
    errno = 0;
  } else {
    handleError("");
  }
  return result == SUCCESS;
}

bool isFilenameWithinLength(const char *filename) {
  return strlen(filename) <= NAME_MAX;
}

char *getValidFilename() {
  char *filename = readStringWithFilterUntilValid(
      "Name of file with records", MAX_FILENAME_LENGTH, filenameReject);

  if (!filename)
    return NULL;

  if (filename[0] == '\0') {
    handleError("String should have at least one printable character.");
    free(filename);
    return NULL;
  }

  if (findCharsInUtf8String(filename, filenameReject) !=
      getUtf8StringLength(filename)) {
    handleError("String contains one of forbidden symbols: \"%s\"",
                filenameReject);
    free(filename);
    return NULL;
  }
  size_t disallowedNamesLength =
      sizeof(disallowedNames) / sizeof(disallowedNames[0]);
  for (size_t i = 0; i < disallowedNamesLength; i++) {

    if (compareUtf8Strings(filename, disallowedNames[i]) == 0) {
      handleError("Name %s is not allowed in this OS.", disallowedNames[i],
                  filename);
      free(filename);
      return NULL;
    }
  }

  if (hasSuffix(filename, ".c") || hasSuffix(filename, ".h")) {
    handleError("It's forbidden to create or edit files with extensions of C "
                "Programming Language.");
    free(filename);
    return NULL;
  }

  return filename;
}

int handleFileIfNotExist(FileContext *context) {
  if (!context) {
    handleError("Context is NULL");
    return FAILURE;
  }
  if (context->file == NULL) {
    handleErrorFileNotSpecified();
    if (!askQuestion("Do you want to create/select file?"))
      return FAILURE;
    if (actionCreateFile(context) == FAILURE)
      return FAILURE;
  }
  return SUCCESS;
}

int handleFileIfEmpty(FileContext *context) {
  if (handleFileIfNotExist(context) == FAILURE)
    return FAILURE;

  if (skipFileHeader(context) == FAILURE)
    return FAILURE;

  char *line = NULL;
  int result = 0;
  while ((result = readLine(&line, DEFAULT_STRING_LENGTH, INFINITE_LENGTH,
                            context->file)) != EOF) {
    if (result == FAILURE) {
      if (line)
        free(line);
      return FAILURE;
    }

    if (!isLineEmpty(line)) {
      free(line);
      return SUCCESS;
    }
    free(line);
  }

  handleError("This file is empty, consider creating some records first.");
  if (!askQuestion("Do you want to create a record?"))
    return FAILURE;
  return actionCreateRecord(context);
}

int handleFlushing(FileContext *context) {
  if (!context->file)
    return FAILURE;

  if (fflush(context->file) == EOF) {
    handleError("Failed to flush the data into %s", context->filename);
    return FAILURE;
  }

  return SUCCESS;
}

// OUTPUT

int handleErrorFileNotSpecified() {
  return handleError("No file with records was chosen. Please, create file "
                     "with records or select it.");
}

int handleBadFileHeader(FileContext *context) {
  if (handleFileIfNotExist(context) == FAILURE)
    return FAILURE;

  rewind(context->file);
  char buffer[context->signatureSize + 1];

  if (!fgets(buffer, context->signatureSize + 1, context->file)) {
    handleError("Failed to read signature from file!");
    return FAILURE;
  }

  if (strcmp(buffer, context->signature) != 0) {
    handleError("File %s doesn't contain the signature %s!", context->filename,
                context->signature);
    closeFileContext(context);
    return FAILURE;
  }

  return SUCCESS;
}

int writeSignature(FileContext *context) {
  if (!context->file) {
    handleErrorFileNotSpecified();
    return FAILURE;
  }

  if (fprintf(context->file, "%s", context->signature) < 0) {
    handleError("Failed to write signature to the file %s", context->filename);
    return FAILURE;
  }

  if (handleFlushing(context) == FAILURE)
    return FAILURE;

  return SUCCESS;
}

int skipFileHeader(FileContext *context) {
  if (handleFileIfNotExist(context) == FAILURE)
    return FAILURE;

  if (fseek(context->file, context->signatureSize, SEEK_SET) != SUCCESS) {
    handleError("Couldn't go to the position after signature.");
    return FAILURE;
  }

  return SUCCESS;
}

FileContext *createTempFile() {
  if (fileExists(TEMP)) {
    if (remove(TEMP) != SUCCESS) {
      handleError("Failed to remove the old temporary file");
      return NULL;
    }
  }

  FileContext *temp = initFileContext();
  if (!temp)
    return NULL;

  if (fillFileContext(temp, TEMP, SIGNATURE) == FAILURE) {
    closeFileContext(temp);
    free(temp);
    return NULL;
  }

  return temp;
}

int truncateFileFromCurrentPosition(FileContext *context) {
  if (handleFileIfNotExist(context) == FAILURE)
    return FAILURE;

  long position = ftell(context->file);

  if (position == -1) {
    handleError("Failed to get the position in file.");
    return FAILURE;
  }

  int fd = fileno(context->file);
  if (fd == -1) {
    handleError("Failed to get the file descriptor.");
    return FAILURE;
  }

  if (ftruncate(fd, position) == -1) {
    handleError("Failed to truncate the file.");
    return FAILURE;
  }
  return SUCCESS;
}
