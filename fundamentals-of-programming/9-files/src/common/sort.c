#include "../common/common.h"
#include "../io/io.h"
#include "../io/utils.h"
#include <string.h>

const char *SORT_BY_CHOICES[COMPARATOR_COUNT] = {
    "by region name",
    "by region area",
    "by region population",
};

const char *SORT_HOW_CHOICES[ORDER_COUNT] = {"ascending", "descending"};

const SortComparator SORT_COMPARATORS[COMPARATOR_COUNT] = {
    compareRegionNames, compareRegionAreas, compareRegionPopulations};

const SortOrder SORT_ORDERS[ORDER_COUNT] = {ASCENDING, DESCENDING};

static SortComparator comparator = NULL;
static SortOrder sortOrder = 0;
int compareRegionNames(const Region *region1, const Region *region2) {
  return compareUtf8Strings(region1->name, region2->name);
}

int compareRegionAreas(const Region *region1, const Region *region2) {
  if (region1->area > region2->area)
    return BIGGER_THAN;
  if (region1->area < region2->area)
    return LESS_THAN;
  return EQUAL;
}

int compareRegionPopulations(const Region *region1, const Region *region2) {
  if (region1->population > region2->population)
    return BIGGER_THAN;
  if (region1->population < region2->population)
    return LESS_THAN;
  return EQUAL;
}

void swap(Region *a, Region *b) {
  if (a == NULL || b == NULL) {
    return;
  }
  Region temp = *a;
  *a = *b;
  *b = temp;
}

size_t partition(Region **arr, size_t low, size_t high) {
  Region *pivot = arr[high];
  size_t i = low - 1;

  for (size_t j = low; j < high; j++) {
    int comparison = comparator(arr[j], pivot);
    if ((sortOrder == ASCENDING && comparison < 0) ||
        (sortOrder == DESCENDING && comparison > 0)) {
      i++;
      swap(arr[i], arr[j]);
    }
  }

  swap(arr[i + 1], arr[high]);
  return i + 1;
}

void quickSort(Region **arr, size_t low, size_t high) {
  if (low >= high) {
    return;
  }
  size_t pivotIndex = partition(arr, low, high);
  if (pivotIndex > 0) {
    quickSort(arr, low, pivotIndex - 1);
  }
  quickSort(arr, pivotIndex + 1, high);
}

void setQuickSortComparator(SortComparator newComparator) {
  static SortComparator *comparatorPtr = NULL;
  if (!comparatorPtr) {
    comparatorPtr = &comparator;
  }
  *comparatorPtr = newComparator;
}

void setQuickSortOrder(SortOrder newSortOrder) {
  static SortOrder *sortOrderPtr = NULL;
  if (!sortOrderPtr) {
    sortOrderPtr = &sortOrder;
  }
  *sortOrderPtr = newSortOrder;
}

SortComparator getQuickSortComparator() { return comparator; }
SortOrder getQuickSortOrder() { return sortOrder; }

bool detectSort(Region **regions, size_t regionCount) {
  if (getQuickSortComparator()) {
    return true;
  }
  bool isSorted = true;
  for (size_t comp = 0; comp < COMPARATOR_COUNT; comp++) {
    for (size_t order = 0; order < ORDER_COUNT; order++) {
      for (size_t ri = 1; ri < regionCount; ri++) {
        if ((SORT_COMPARATORS[comp])(regions[ri - 1], regions[ri]) *
                SORT_ORDERS[order] >
            0) { // If order is broken
          isSorted = false;
          break;
        }
      }

      if (isSorted) {
        setQuickSortComparator(SORT_COMPARATORS[comp]);
        setQuickSortOrder(ASCENDING);
        return true;
      }
      isSorted = true;
    }
  }

  return false;
}

void showCurrentSortMethod() {
  SortComparator comp = getQuickSortComparator();
  SortOrder sortOrder = getQuickSortOrder();
  size_t compI = 0;
  size_t orderI = 0;

  for (; compI < COMPARATOR_COUNT; compI++) {
    if (SORT_COMPARATORS[compI] == comp) {
      break;
    }
  }
  for (; orderI < ORDER_COUNT; orderI++) {
    if (SORT_ORDERS[orderI] == sortOrder) {
      break;
    }
  }

  printf("Current sorting method is: \n");
  showSuccess("%s %s", SORT_BY_CHOICES[compI], SORT_HOW_CHOICES[orderI]);
  printf("\n");
}
