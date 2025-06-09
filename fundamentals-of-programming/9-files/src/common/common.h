#ifndef COMMON_H
#define COMMON_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

#define SUCCESS 0
#define FAILURE 1
#define NOOP 2
#define DISPLAY_MENU 3
#define EXIT -2 // INFO: to not be equal EOF = -1
#define PIPE 1

typedef struct {
  char *name;
  double area;
  double population;
} Region;

// INPUT
#define INFINITE_LENGTH 0
#define DEFAULT_STRING_LENGTH 16
typedef enum { TYPE_DOUBLE, TYPE_SIZE_T, TYPE_COUNT } NumberType;

typedef struct {
  void *min;
  void *max;
  bool isMinIncluded;
  bool isMaxIncluded;
  char *valueName;
  int (*compare)(const void *, const void *);
  NumberType type;
} NumberRange;

typedef enum {
  LESS,
  LESS_EQUAL,
  GREATER,
  GREATER_EQUAL,
  WITHIN_RANGE
} RangeCheckResult;

typedef bool(ValidationFunc)(void *, va_list);
// FILE
#define SIGNATURE "RegionSimulator3000\n"
#define TEMP "temp.region"
#define MAX_FILENAME_LENGTH 255

typedef struct {
  FILE *file;
  char *filename;
  char *signature;
  size_t signatureSize;
} FileContext;

typedef int (*Action)(FileContext *context);

FileContext *initFileContext();
int fillFileContext(FileContext *context, const char *filename,
                    const char *signature);
int closeFileContext(FileContext *context);

// SORTING
#define BIGGER_THAN 1
#define LESS_THAN -1
#define EQUAL 0
#define COMPARATOR_COUNT 3
#define ORDER_COUNT 2

typedef enum { DESCENDING = -1, ASCENDING = 1 } SortOrder;
typedef int (*SortComparator)(const Region *, const Region *);

int compareRegionNames(const Region *region1, const Region *region2);
int compareRegionAreas(const Region *region1, const Region *region2);
int compareRegionPopulations(const Region *region1, const Region *region2);

void quickSort(Region **arr, size_t low, size_t high);
bool detectSort(Region **regions, size_t regionCount);
SortComparator getQuickSortComparator();
SortOrder getQuickSortOrder();
void setQuickSortOrder(SortOrder newSortOrder);
void setQuickSortComparator(SortComparator newComparator);

extern const char *SORT_BY_CHOICES[COMPARATOR_COUNT];
extern const char *SORT_HOW_CHOICES[ORDER_COUNT];
extern const SortComparator SORT_COMPARATORS[COMPARATOR_COUNT];
extern const SortOrder SORT_ORDERS[ORDER_COUNT];

// STACK AND MENU
typedef struct StackNode {
  void *data;
  struct StackNode *next;
} StackNode;

typedef struct {
  struct StackNode *top;
  size_t size;
} Stack;

Stack *createStack();
void freeStack(Stack *stack);
int push(Stack *stack, void *menu);
void *pop(Stack *stack);
void *peek(Stack *stack);

typedef struct MenuNode {
  char *name;
  struct MenuNode *child;
  struct MenuNode *sibling;
  int option;
  Action action;
} MenuNode;
#endif
