#include "../actions/utils.h"
#include "../io/utils.h"
#include "common.h"
#include <stdlib.h>
#include <string.h>

FileContext *initFileContext() {
  FileContext *context = malloc(sizeof(FileContext));
  if (!context) {
    handleErrorMemoryAllocation("file context");
    return NULL;
  }

  context->file = NULL;
  context->filename = NULL;
  context->signature = NULL;
  context->signatureSize = 0;

  return context;
}

int closeFileContext(FileContext *context) {
  if (context == NULL) {
    handleError("Context can't be NULL before closing: nothing to close");
    return FAILURE;
  }

  if (context->file) {
    fclose(context->file);
    context->file = NULL;
  }

  if (context->filename) {
    free(context->filename);
    context->filename = NULL;
  }

  if (context->signature) {
    free(context->signature);
    context->signature = NULL;
  }
  return SUCCESS;
}

int fillFileContext(FileContext *context, const char *filename,
                    const char *signature) {
  context->filename = strdup(filename);
  if (context->filename == NULL) {
    handleErrorMemoryAllocation("filename string");
    closeFileContext(context);
    return FAILURE;
  }

  context->signature = strdup(signature);
  if (context->signature == NULL) {
    handleErrorMemoryAllocation("signature string");
    closeFileContext(context);
    return FAILURE;
  }

  context->signatureSize = strlen(signature);

  bool doesExist = fileExists(filename);
  const char *mode = (doesExist) ? "r+" : "w+";

  context->file = fopen(filename, mode);
  if (!context->file) {
    handleError("Failed to create new file.");
    closeFileContext(context);
    return FAILURE;
  }

  if (!doesExist) { // Writing signature to new file
    if (writeSignature(context) == FAILURE) {
      closeFileContext(context);
      return FAILURE;
    }
  }

  return SUCCESS;
}
