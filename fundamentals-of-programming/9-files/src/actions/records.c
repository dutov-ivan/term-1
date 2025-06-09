#include "../actions/utils.h"
#include "../common/common.h"
#include "../io/io.h"
#include "../io/utils.h"
#include "actions.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

int actionCreateRecord(FileContext *context) {
  if (handleFileIfNotExist(context) == FAILURE)
    return FAILURE;

  if (skipFileHeader(context) == FAILURE)
    return FAILURE;

  if (fseek(context->file, 0L, SEEK_END) != SUCCESS) {
    handleError("Couldn't go to the bottom of file");
    return FAILURE;
  }

  Region *region = getRegionFromUser();
  if (!region)
    return FAILURE;

  if (writeRegion(region, context) == FAILURE)
    return FAILURE;

  printRegion(region, 0); // without number
  freeRegion(region);
  setQuickSortComparator(NULL); // Sort may be removed
  showSuccess("Record was appended successfully.");

  return SUCCESS;
}

int actionReadRecord(FileContext *context) {
  if (handleFileIfNotExist(context) == FAILURE)
    return FAILURE;

  if (handleFileIfEmpty(context) == FAILURE)
    return FAILURE;

  if (skipFileHeader(context) == FAILURE)
    return FAILURE;

  size_t recordNumber = 0;
  if (readNumberWithValidation(&recordNumber, TYPE_SIZE_T, "Record number",
                               isNotZero) == EOF)
    return EXIT;

  Region *region = readRegionAt(recordNumber, context);

  if (!region)
    return FAILURE;

  printRegion(region, recordNumber);
  freeRegion(region);

  showSuccess("Record read successfully");
  return SUCCESS;
}

int actionEditRecord(FileContext *context) {
  if (handleFileIfNotExist(context) == FAILURE)
    return FAILURE;

  if (handleFileIfEmpty(context) == FAILURE)
    return FAILURE;

  size_t recordNumber;
  readNumberWithValidation(&recordNumber, TYPE_SIZE_T, "Record number",
                           isNotZero);

  Region *regionBeforeEdited = readRegionAt(recordNumber - 1, context);
  if (!regionBeforeEdited)
    return FAILURE;
  freeRegion(regionBeforeEdited);

  long positionBeforeEdited = ftell(context->file);
  if (positionBeforeEdited < 0) {
    handleError("Failed to move file pointer to position before edited.");
    return FAILURE;
  }

  Region *region = initRegion();
  if (!region)
    return FAILURE;

  int readResult = readRegion(region, context);
  if (readResult != SUCCESS) {
    if (readResult == EOF)
      handleError("File doesn't contain region %zu", recordNumber);
    freeRegion(region);
    return FAILURE;
  }

  int editResult = editRegion(region, recordNumber);
  if (editResult == FAILURE) {
    freeRegion(region);
    return FAILURE;
  }
  if (editResult == NOOP) { // No changes were made
    showSuccess("No changes were made to the region so far.");
    freeRegion(region);
    return SUCCESS;
  }

  FileContext *temp = createTempFile();
  if (!temp)
    goto cleanup;

  if (copyRegionsBetweenFiles(context, temp) == FAILURE)
    goto cleanup;

  if (fseek(context->file, positionBeforeEdited, SEEK_SET) != SUCCESS) {
    handleError("Couldn't move back to the position before edited record");
    goto cleanup;
  }

  if (writeRegion(region, context) == FAILURE)
    goto cleanup;

  if (skipFileHeader(temp) == FAILURE)
    goto cleanup;
  if (copyRegionsBetweenFiles(temp, context) == FAILURE)
    goto cleanup;

  if (truncateFileFromCurrentPosition(context) == FAILURE)
    goto cleanup;

  if (handleFlushing(context) == FAILURE)
    goto cleanup;

  setQuickSortComparator(NULL); // The sorting might've been removed
  freeRegion(region);
  closeFileContext(temp);
  free(temp);
  temp = NULL;

  if (remove(TEMP) != SUCCESS) {
    handleError("Failed to delete temporary file.");
    return FAILURE;
  }

  showSuccess("Record was edited successfully.");
  return SUCCESS;

cleanup:
  if (temp) {
    closeFileContext(temp);
    free(temp);
    if (remove(TEMP) != SUCCESS) {
      handleError("Failed to delete temporary file.");
    }
  }
  freeRegion(region);
  return FAILURE;
}

int actionSortRecords(FileContext *context) {
  if (handleFileIfNotExist(context) == FAILURE)
    return FAILURE;

  if (handleFileIfEmpty(context) == FAILURE)
    return FAILURE;

  if (skipFileHeader(context) == FAILURE)
    return FAILURE;

  size_t regionCount = 0;
  Region **regions = readAllRegions(&regionCount, context);

  if (!regions || regionCount == 0) {
    handleError(
        "Record file doesn't contain any regions, or they are malformed!");
    return FAILURE;
  }

  if (regionCount == 1) {
    showSuccess("The file contains 1 entry. It's already sorted.");
    return SUCCESS;
  }

  bool isSorting = true;

  NumberRange *sortOptionRange = initChoiceRange(1, COMPARATOR_COUNT);
  if (!sortOptionRange)
    goto cleanup;

  do {
    size_t sortOption = getUserChoice(sortOptionRange, "Choose sorting method",
                                      SORT_BY_CHOICES) -
                        1;
    setQuickSortComparator(SORT_COMPARATORS[sortOption]);

    setQuickSortOrder(
        askQuestion("Do you want to sort ascending?") ? ASCENDING : DESCENDING);

    quickSort(regions, 0, regionCount - 1);

    for (size_t i = 0; i < regionCount; i++) {
      printRegion(regions[i], i + 1);
    }

    if (askQuestion("Do you want to write to the file?"))
      break;

    if (askQuestion("Do you want to sort it different way?"))
      continue;

    if (!askQuestion("Do you even want to continue?"))
      goto cleanup;
  } while (isSorting);

  freeNumberRange(sortOptionRange);

  if (skipFileHeader(context) == FAILURE)
    goto cleanup;

  for (size_t i = 0; i < regionCount; i++) {
    int result;
    if ((result = writeRegion(regions[i], context)) == FAILURE) {
      if (result == FAILURE)
        goto cleanup;
    }
  }

  if (handleFlushing(context) == FAILURE)
    goto cleanup;

  showSuccess("Sorted regions were written successfully!");

  for (size_t i = 0; i < regionCount; i++)
    freeRegion(regions[i]);

  free(regions);
  return SUCCESS;

cleanup:
  if (regions)
    for (size_t i = 0; i < regionCount; i++) {
      freeRegion(regions[i]);
    }
  free(regions);
  return FAILURE;
}

int actionInsertRecordIfSorted(FileContext *context) {
  if (handleFileIfNotExist(context) == FAILURE)
    return FAILURE;

  if (handleFileIfEmpty(context) == FAILURE)
    return FAILURE;

  if (skipFileHeader(context) == FAILURE)
    return FAILURE;

  size_t regionCount = 0;
  Region **regions = readAllRegions(&regionCount, context);
  if (regions == NULL)
    return FAILURE;

  if (!detectSort(regions, regionCount)) {
    handleError("The file you provided is not sorted in any regular way.");
    if (askQuestion("Do you want to sort it?")) {
      if (actionSortRecords(context) == FAILURE)
        goto cleanup;
    }
  }

  showCurrentSortMethod();

  Region *newRegion = getRegionFromUser();
  if (!newRegion)
    goto cleanup;

  regionCount++;
  Region **newRegions = realloc(regions, regionCount * sizeof(Region *));
  if (newRegions == NULL) {
    handleErrorMemoryAllocation("extending region array");
    goto cleanup;
  }
  regions = newRegions;

  regions[regionCount - 1] = newRegion;
  quickSort(regions, 0, regionCount - 1);

  if (skipFileHeader(context) == FAILURE)
    goto cleanup;

  for (size_t i = 0; i < regionCount; i++) {
    if (writeRegion(regions[i], context) == FAILURE)
      goto cleanup;
  }

  for (size_t i = 0; i < regionCount; i++)
    freeRegion(regions[i]);
  free(regions);

  showSuccess("Inserted item successfully");
  return SUCCESS;

cleanup:
  for (size_t i = 0; i < regionCount; i++)
    freeRegion(regions[i]);
  free(regions);
  return FAILURE;
}

int actionDeleteRecord(FileContext *context) {
  if (handleFileIfNotExist(context) == FAILURE)
    return FAILURE;

  if (handleFileIfEmpty(context) == FAILURE)
    return FAILURE;

  if (skipFileHeader(context) == FAILURE)
    return FAILURE;

  size_t recordNumber;
  if (readNumberWithValidation(&recordNumber, TYPE_SIZE_T, "Record number",
                               isNotZero) == EOF)
    return FAILURE;

  Region *regionBeforeDeleted = readRegionAt(recordNumber - 1, context);
  if (!regionBeforeDeleted)
    return FAILURE;

  freeRegion(regionBeforeDeleted);

  long positionBeforeDeleted = ftell(context->file);

  Region *deleteRegion = initRegion();
  if (!deleteRegion)
    return FAILURE;

  if (readRegion(deleteRegion, context) != SUCCESS) {
    freeRegion(deleteRegion);
    return FAILURE;
  }

  printRegion(deleteRegion, recordNumber);
  freeRegion(deleteRegion);

  if (!askQuestion("Do you REALLY want to delete this region?"))
    return FAILURE;

  FileContext *temp = createTempFile();
  if (!temp)
    goto cleanup;

  if (copyRegionsBetweenFiles(context, temp) == FAILURE)
    goto cleanup;

  if (fseek(context->file, positionBeforeDeleted, SEEK_SET) != SUCCESS) {
    handleError("Failed to move to the position before deleted record.");
    goto cleanup;
  }

  if (skipFileHeader(temp) == FAILURE)
    goto cleanup;

  if (copyRegionsBetweenFiles(temp, context) == FAILURE)
    goto cleanup;

  if (truncateFileFromCurrentPosition(context) == FAILURE)
    goto cleanup;

  closeFileContext(temp);
  free(temp);
  if (remove(TEMP) != SUCCESS) {
    handleError("Failed to delete temporary file");
    goto cleanup;
  }

  if (handleFlushing(context) == FAILURE)
    goto cleanup;

  showSuccess("Record was deleted successfully.");
  return SUCCESS;
cleanup:
  if (temp) {
    closeFileContext(temp);
    free(temp);
    if (remove(TEMP) != SUCCESS) {
      handleError("Failed to delete temporary file");
    }
  }
  return FAILURE;
}
