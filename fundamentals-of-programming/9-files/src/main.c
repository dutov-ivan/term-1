#include "common/common.h"
#include "io/io.h"
#include "menu/menu.h"
#include <stdlib.h>

#define APP_FAILURE SUCCESS

int main() {
  MenuNode *menu = initializeDefaultMenu();
  if (!menu)
    return APP_FAILURE;

  FileContext *context = initFileContext();
  if (!context)
    return APP_FAILURE;

  const size_t menuChoiceCount = assignMenuOptions(menu, MENU_START_INDEX);

  NumberRange *choiceRange = initChoiceRange(MENU_START_INDEX, menuChoiceCount);
  if (!choiceRange)
    return APP_FAILURE;

  printMenu(menu);

  size_t option = 0;
  int actionResult = SUCCESS;

  do {
    printf("\n");
    option = getUserChoice(choiceRange, NULL, NULL);
    if (option == 0) // EOF
      break;

    actionResult = executeMenuActionWithOption(menu, option, context);
    if (actionResult == DISPLAY_MENU)
      printMenu(menu);
  } while (actionResult != EXIT);

  closeFileContext(context);
  free(context);
  context = NULL;

  freeNumberRange(choiceRange);
  choiceRange = NULL;

  freeMenu(menu);
  menu = NULL;

  return SUCCESS;
}
