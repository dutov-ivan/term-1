#include "../actions/actions.h"
#include "menu.h"

MenuNode *initializeDefaultMenu() {
  MenuNode *menu = createMenuNode("Menu", NULL);
  if (!menu)
    return NULL;

  MenuNode *file = NULL, *records = NULL, *createFile = NULL, *readFile = NULL,
           *deleteFile = NULL, *createRecords = NULL, *readRecords = NULL,
           *editRecords = NULL, *sortRecords = NULL, *insertRecords = NULL,
           *deleteRecords = NULL, *exitProgram = NULL, *verifySignature = NULL,
           *displayMenu = NULL;

  // File part
  if (!(file = createMenuNode("File", NULL)) ||
      !(createFile = createMenuNode("Create/Select File", actionCreateFile)) ||
      !(readFile = createMenuNode("Read all Records", actionReadFile)) ||
      !(deleteFile =
            createMenuNode("Delete/Deselect File", actionDeleteFile)) ||

      // Record part
      !(records = createMenuNode("Records", NULL)) ||
      !(createRecords = createMenuNode("Create Record", actionCreateRecord)) ||
      !(readRecords = createMenuNode("Read Record", actionReadRecord)) ||
      !(editRecords = createMenuNode("Edit Record", actionEditRecord)) ||
      !(sortRecords = createMenuNode("Sort Records", actionSortRecords)) ||
      !(insertRecords =
            createMenuNode("Insert Record", actionInsertRecordIfSorted)) ||
      !(deleteRecords = createMenuNode("Delete Record", actionDeleteRecord)) ||

      // Misc part
      !(exitProgram = createMenuNode("Exit Program", actionExitProgram)) ||
      !(verifySignature =
            createMenuNode("Verify File Header", actionVerifyFileHeader)) ||
      !(displayMenu =
            createMenuNode("Display Menu Again", actionDisplayMenu))) {
    freeMenu(menu);
    return NULL;
  }

  addMenuChild(menu, file);
  addMenuChild(menu, records);
  addMenuChild(menu, exitProgram);
  addMenuChild(menu, verifySignature);
  addMenuChild(menu, displayMenu);

  addMenuChild(file, createFile);
  addMenuChild(file, readFile);
  addMenuChild(file, deleteFile);

  addMenuChild(records, createRecords);
  addMenuChild(records, readRecords);
  addMenuChild(records, editRecords);
  addMenuChild(records, sortRecords);
  addMenuChild(records, insertRecords);
  addMenuChild(records, deleteRecords);

  assignMenuOptions(menu, MENU_START_INDEX);

  return menu;
}
