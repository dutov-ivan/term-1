#include "menu.h"
#include "../common/common.h"
#include "../io/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TREE_SPACING 2

MenuNode *createMenuNode(const char *name, Action action) {
  MenuNode *node = (MenuNode *)malloc(sizeof(MenuNode));

  if (!node) {
    handleErrorMemoryAllocation("menu node");
    return NULL;
  }

  node->name = strdup(name);
  if (!node->name) {
    free(node);
    handleErrorMemoryAllocation("menu node name");
    return NULL;
  }

  node->action = action;
  node->child = NULL;
  node->sibling = NULL;

  return node;
}

void addMenuChild(MenuNode *parent, MenuNode *child) {
  if (!parent->child) {
    parent->child = child;
    return;
  }

  MenuNode *temp = parent->child;
  while (temp->sibling) {
    temp = temp->sibling;
  }
  temp->sibling = child;
  child->sibling = NULL;
}

int executeMenuActionWithOption(MenuNode *menu, int option,
                                FileContext *context) {
  Stack *stack = createStack();
  if (!stack) {
    handleErrorMemoryAllocation("stack for menu");
    return FAILURE;
  }

  push(stack, menu);
  while (stack->size != 0) {
    MenuNode *cur = pop(stack);
    if (cur->action && cur->option == option) {
      freeStack(stack);
      return cur->action(context);
    }

    if (cur->child) {
      MenuNode *child = cur->child;
      while (child) {
        push(stack, child);
        child = child->sibling;
      }
    }
  }
  freeStack(stack);
  return FAILURE; // specified option not found.
}

int assignMenuOptions(MenuNode *node, int start) {
  if (!node)
    return start - 1;

  int lastItem;
  if (node->action) {
    node->option = start;
    lastItem = assignMenuOptions(node->child, start + 1);
  } else {
    lastItem = assignMenuOptions(node->child, start);
  }

  return assignMenuOptions(node->sibling, lastItem + 1);
}

void freeMenu(MenuNode *node) {
  if (!node)
    return;

  freeMenu(node->child);
  freeMenu(node->sibling);

  free(node->name);
  free(node);
}

void printMenuNode(MenuNode *node) {
  if (!node)
    return;

  if (node->action) {
    printf("%d: %s", node->option, node->name);
  } else {
    printf("%s", node->name);
  }
}

int printMenu(MenuNode *root) {
  if (!root) {
    handleError("Root menu node is NULL.");
    return FAILURE;
  }

  Stack *stack = createStack();
  if (!stack) {
    return FAILURE;
  }

  size_t maxLevels = 10;
  bool *hasSiblings = (bool *)malloc(maxLevels * sizeof(bool));
  if (hasSiblings == NULL) {
    handleErrorMemoryAllocation("array of bool for finding menu node siblings");
    freeStack(stack);
    return FAILURE;
  }

  size_t level = 0;
  push(stack, root);

  while (stack->size != 0) {
    MenuNode *cur = pop(stack);

    // Ensure `hasSiblings` array has enough space
    if (level >= maxLevels) {
      maxLevels *= 2;
      bool *temp = (bool *)realloc(hasSiblings, maxLevels * sizeof(bool));
      if (temp == NULL) {
        handleErrorMemoryAllocation("reallocation of menu node siblings array");
        free(hasSiblings);
        freeStack(stack);
        return FAILURE;
      }
      hasSiblings = temp;
    }

    hasSiblings[level] = (cur->sibling != NULL);

    for (size_t i = 0; i < level; i++) {
      if (hasSiblings[i]) {
        printf("│%-*s ", TREE_SPACING, " ");
      } else {
        printf(" %-*s ", TREE_SPACING, " ");
      }
    }
    if (hasSiblings[level]) {
      printf("├");
    } else {
      printf("└");
    }
    printf(" ");
    printMenuNode(cur);

    if (cur->child) {
      MenuNode *child = cur->child;

      // Push siblings in reverse order to preserve the order
      Stack *tempStack = createStack();
      if (!tempStack)
        return FAILURE;

      while (child) {
        push(tempStack, child);
        child = child->sibling;
      }

      while (tempStack->size != 0) {
        push(stack, pop(tempStack));
      }

      freeStack(tempStack);
      level++;
    } else if (!cur->sibling) {
      level--; // Go back up if no siblings remain
    }
    printf("\n");
  }

  free(hasSiblings);
  freeStack(stack);
  return SUCCESS;
}
