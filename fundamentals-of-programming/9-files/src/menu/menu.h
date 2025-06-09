#ifndef MENU_H

#include "../common/common.h"
#include <stdbool.h>

#define MENU_START_INDEX 1
MenuNode *createMenuNode(const char *name, Action action);
void addMenuChild(MenuNode *parent, MenuNode *child);
int printMenu(MenuNode *root);
void freeMenu(MenuNode *node);
int executeMenuActionWithOption(MenuNode *menu, int option,
                                FileContext *context);
int assignMenuOptions(MenuNode *node, int start);

MenuNode *initializeDefaultMenu();
#endif // !MENU_H
