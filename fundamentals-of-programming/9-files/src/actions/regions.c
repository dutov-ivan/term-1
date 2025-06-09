#include "../common/common.h"
#include "../io/io.h"
#include "../io/utils.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>
#define MAX_SIGNIFICANT_DIGITS 15

static double minArea = 1e-6;
static double maxArea = 510e6;
static const NumberRange areaRange = {
    &minArea, &maxArea, true, true, "Region area", compareDouble, TYPE_DOUBLE};

static double minPopulation = 1e-6;
static double maxPopulation = 1e10;

static const NumberRange populationRange = {
    &minPopulation,      &maxPopulation, true,       true,
    "Region population", compareDouble,  TYPE_DOUBLE};

Region *initRegion() {
  Region *region = malloc(sizeof(Region));
  if (!region) {
    handleErrorMemoryAllocation("region");
    return NULL;
  }

  region->name = NULL;
  region->area = 0.0;
  region->population = 0.0;
  return region;
}

void freeRegion(Region *region) {
  if (region->name) {
    free(region->name);
    region->name = NULL;
  }
  if (region)
    free(region);
}

int writeRegion(Region *region, FileContext *context) {
  if (handleFileIfNotExist(context) == FAILURE)
    return FAILURE;

  if (fprintf(context->file, "%s,%.*lg,%.*lg\n", region->name,
              MAX_SIGNIFICANT_DIGITS, region->area, MAX_SIGNIFICANT_DIGITS,
              region->population) < 0) {
    handleError("Failed to write to file %s.", context->filename);
    return FAILURE;
  }

  if (handleFlushing(context) == FAILURE)
    return FAILURE;
  return SUCCESS;
}

int readRegion(Region *region, FileContext *context) {
  if (!region) {
    handleError("Region is NULL in readRegion");
    return FAILURE;
  }
  if (handleFileIfNotExist(context) == FAILURE)
    return FAILURE;

  char *buffer = NULL;

  int result = SUCCESS;
  do {
    if (buffer)
      free(buffer);
    result = readLine(&buffer, DEFAULT_STRING_LENGTH, INFINITE_LENGTH,
                      context->file);
    if (result == EOF) {
      if (buffer) {
        free(buffer);
        buffer = NULL;
      }
      return EOF;
    }
    if (result == FAILURE) {
      handleError("Failed to read line in file %s.", context->filename);
      goto cleanup;
    }
  } while (isLineEmpty(buffer));

  // Locate commas
  char *comma1 = strchr(buffer, ',');
  char *comma2 = comma1 ? strchr(comma1 + 1, ',') : NULL;
  char *comma3 = comma2 ? strchr(comma2 + 1, ',') : NULL;

  if (!comma1 || !comma2) {
    handleError("Line doesn't contain enough commas.");
    goto cleanup;
  }
  if (comma3) {
    handleError("Line contains too many commas.");
    goto cleanup;
  }

  // Extract and allocate name
  size_t nameLength = (size_t)(comma1 - buffer);
  region->name = calloc(1 + nameLength, sizeof(char));

  if (!region->name) {
    handleErrorMemoryAllocation("region name string");
    goto cleanup;
  }
  strncpy(region->name, buffer, nameLength);
  if (!trimWhitespaceUtf8(region->name))
    goto cleanup;

  if (region->name[0] == '\0') {
    handleError(
        "Region name became empty string after whitespace was trimmed.");
    goto cleanup;
  }

  region->name[nameLength] = '\0';

  // Parse area
  char *endPtr = NULL;
  region->area = strtod(comma1 + 1, &endPtr);
  if (endPtr == comma1 + 1 || endPtr != comma2) {
    handleError("Region area is not a valid number.");
    goto cleanup;
  }

  if (validateNumberRange(&region->area, &areaRange) != WITHIN_RANGE)
    goto cleanup;

  // Parse population
  endPtr = NULL;
  region->population = strtod(comma2 + 1, &endPtr);

  if (endPtr == comma2 + 1 || *endPtr != '\0') {
    handleError("Region population is not a valid number.");
    goto cleanup;
  }

  if (validateNumberRange(&region->population, &populationRange) !=
      WITHIN_RANGE)
    goto cleanup;

  if (buffer)
    free(buffer);
  return SUCCESS;

cleanup:
  if (buffer) {
    free(buffer);
    buffer = NULL;
  }
  return FAILURE;
}

Region *getRegionFromUser() {
  Region *region = initRegion();
  if (region == NULL)
    return NULL;
  region->name = readStringWithFilterUntilValid("Region name (without commas)",
                                                INFINITE_LENGTH, ",");
  if (!region->name) {
    free(region);
    return NULL;
  }

  if (readNumberWithinRangeWithValidation(&region->area, TYPE_DOUBLE,
                                          &areaRange, areaRange.valueName,
                                          NULL) == EOF) {
    free(region);
    return NULL;
  }

  if (readNumberWithinRangeWithValidation(
          &region->population, TYPE_DOUBLE, &populationRange,
          populationRange.valueName, NULL) == EOF) {
    free(region);
    return NULL;
  }

  return region;
}

void printRegion(Region *region, size_t regionNumber) {
  printf("\n");
  if (regionNumber != 0)
    printf("%zu) ", regionNumber);
  printf("Name: %s\n", region->name);
  printf("Area: %.*lg\n", MAX_SIGNIFICANT_DIGITS, region->area);
  printf("Population: %.*lg\n", MAX_SIGNIFICANT_DIGITS, region->population);
  printf("\n");
}

Region **readAllRegions(size_t *regionCount, FileContext *context) {
  if (handleFileIfNotExist(context) == FAILURE)
    return NULL;

  if (!regionCount) {
    handleError("Region count is NULL.");
    return NULL;
  }

  size_t count = 0;
  size_t capacity = 4;
  size_t regionSize = capacity * sizeof(Region *);

  Region **regions = malloc(regionSize);
  if (!regions) {
    handleErrorMemoryAllocation("regions array");
    return NULL;
  }

  Region *newRegion = initRegion();
  if (!newRegion) {
    handleErrorMemoryAllocation("new region");
    goto cleanup;
  }

  int result = 0;

  while ((result = readRegion(newRegion, context)) != EOF) {
    if (result == FAILURE)
      goto cleanup;

    if (count == capacity) {
      capacity *= 2;
      Region **newBuffer = realloc(regions, capacity * sizeof(Region *));
      if (!newBuffer) {
        handleErrorMemoryAllocation("regions array resize");
        goto cleanup;
      }
      regions = newBuffer;
    }

    regions[count] = newRegion;
    count++;

    newRegion = initRegion();
    if (!newRegion) {
      handleErrorMemoryAllocation("new region");
      goto cleanup;
    }
  }

  free(newRegion);
  *regionCount = count;
  return regions;

cleanup:
  for (size_t i = 0; i < count; i++)
    freeRegion(regions[i]);
  free(regions);
  if (newRegion)
    freeRegion(newRegion);
  return NULL;
}

int editRegion(Region *region, size_t regionNumber) {
  char *nameBuffer = NULL;
  double newArea = region->area;
  double newPopulation = region->population;

  printRegion(region, regionNumber);

  if (askQuestion("Do you want to rename this region?")) {
    nameBuffer =
        readStringWithFilterUntilValid("Region name", INFINITE_LENGTH, ",");
    if (!nameBuffer)
      return FAILURE;
  }

  if (askQuestion("Do you want to change region area?"))
    if (readNumberWithinRangeWithValidation(&newArea, TYPE_DOUBLE, &areaRange,
                                            areaRange.valueName, NULL) == EOF)
      return FAILURE;

  if (askQuestion("Do you want to change region population?"))
    if (readNumberWithinRangeWithValidation(
            &newPopulation, TYPE_DOUBLE, &populationRange,
            populationRange.valueName, NULL) == FAILURE)
      return FAILURE;

  if (newArea == region->area && newPopulation == region->population &&
      nameBuffer == NULL)
    return NOOP;

  if (nameBuffer) {
    if (region->name)
      free(region->name);
    region->name = nameBuffer;
  }

  region->area = newArea;
  region->population = newPopulation;

  return SUCCESS;
}

int copyRegionsBetweenFiles(FileContext *source, FileContext *destination) {
  Region *region = initRegion();

  int result;
  while ((result = readRegion(region, source)) != EOF) {
    if (result == FAILURE || writeRegion(region, destination) == FAILURE) {
      freeRegion(region);
      return FAILURE;
    }
    freeRegion(region);
    region = initRegion();
  }

  freeRegion(region);

  return SUCCESS;
}

Region *readRegionAt(size_t position, FileContext *context) {
  if (handleFileIfNotExist(context) == FAILURE)
    return NULL;

  if (skipFileHeader(context) == FAILURE)
    return NULL;

  Region *region = initRegion();
  if (!region)
    return NULL;

  if (position == 0) {
    return region; // Noop (e.g. already passed nothing)
  }

  size_t i = 0;
  while (i != position) {
    int result = readRegion(region, context);
    if (result == EOF) {
      handleError("File doesn't contain region with such number");
      freeRegion(region);
      return NULL;
    }

    if (result == FAILURE) {
      freeRegion(region);
      region = initRegion();
      i++;
      continue;
    }

    i++;
    if (i == position) {
      return region;
    }

    freeRegion(region);
    region = initRegion();
    if (!region)
      return NULL;
  }

  if (!region->name) { // If nothing was found
    freeRegion(region);
    return NULL;
  }

  return region;

cleanup:
  freeRegion(region);
  return NULL;
}
