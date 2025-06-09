#include "../common/common.h"
#include "../io/utils.h"
#include "actions.h"
#include "assert.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int actionCreateFile(FileContext *context) {
  if (!context) {
    handleError("Context is NULL");
    return FAILURE;
  }

  if (context->file) {
    handleError("Can't create new file while other is already available.");
    if (actionDeleteFile(context) == FAILURE)
      return FAILURE;
  }

  char *filename = getValidFilename();
  if (!filename)
    return FAILURE;

  bool doesExist = fileExists(filename);
  const char *mode = doesExist ? "r+" : "w+";

  if (fillFileContext(context, filename, SIGNATURE) == FAILURE)
    goto cleanup;

  showSuccess("File was %s successfully.", doesExist ? "selected" : "created");

  if (handleBadFileHeader(context) == FAILURE)
    goto cleanup;

  free(filename);
  return SUCCESS;

cleanup:
  if (filename)
    free(filename);
  return FAILURE;
}

int actionReadFile(FileContext *context) {
  if (handleFileIfNotExist(context) == FAILURE)
    return FAILURE;

  if (handleFileIfEmpty(context) == FAILURE)
    return FAILURE;

  if (skipFileHeader(context) == FAILURE)
    return FAILURE;

  Region *region = initRegion();

  if (!region)
    return FAILURE;

  size_t processedRegionCount = 0;
  int result = SUCCESS;
  while ((result = readRegion(region, context)) != EOF) {
    if (result == FAILURE) {
      freeRegion(region);
      region = NULL;
      region = initRegion();
      if (!region)
        return FAILURE;
      showWarning("Improperly formatted region number %lu is skipped.",
                  processedRegionCount + 1);
      processedRegionCount++;
      continue;
    }

    processedRegionCount++;
    printRegion(region, processedRegionCount);
    freeRegion(region);
    region = initRegion();
  }

  freeRegion(region);

  showSuccess("File was read successfully.");
  return SUCCESS;
}

int actionDeleteFile(FileContext *context) {
  if (!context) {
    handleError("Context is NULL");
    return FAILURE;
  }

  if (!context->file) {
    handleErrorFileNotSpecified();
    return FAILURE;
  }

  if (!askQuestion("Do you REALLY want to DELETE this file?")) {
    if (!askQuestion("Maybe you want to deselect it?"))
      return FAILURE;
    closeFileContext(context);
    return SUCCESS;
  }

  fclose(context->file);
  context->file = NULL;

  if (remove(context->filename) != SUCCESS) {
    handleError("Error deleting file with records.");
    return FAILURE;
  }

  closeFileContext(context);
  setQuickSortComparator(NULL);

  showSuccess("File was deleted successfully.", context->filename);
  return SUCCESS;
}
