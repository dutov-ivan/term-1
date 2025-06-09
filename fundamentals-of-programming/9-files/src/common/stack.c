#include "../common/common.h"
#include "../io/utils.h"
#include <stdio.h>
#include <stdlib.h>

Stack *createStack() {
  Stack *stack = (Stack *)malloc(sizeof(Stack));
  if (!stack) {
    handleErrorMemoryAllocation("Stack");
    return NULL;
  }
  stack->size = 0;
  stack->top = NULL;
  return stack;
}

void freeStack(Stack *stack) {
  StackNode *item = stack->top;
  while (item) {
    StackNode *temp = item;
    item = item->next;
    free(temp);
  }
  free(stack);
}

int push(Stack *stack, void *data) {
  StackNode *item = (StackNode *)malloc(sizeof(StackNode));
  if (!item) {
    handleErrorMemoryAllocation("StackNode");
    return FAILURE;
  }
  item->data = data;
  item->next = stack->top;
  stack->top = item;
  stack->size++;
  return SUCCESS;
}

void *pop(Stack *stack) {
  if (stack->size == 0) {
    handleError("Error popping value from the stack");
    return NULL;
  }
  StackNode *temp = stack->top;
  void *data = temp->data;
  stack->top = temp->next;
  free(temp);
  stack->size--;
  return data;
}

void *peek(Stack *stack) {
  if (stack->size == 0) {
    return NULL;
  }
  return stack->top->data;
}
