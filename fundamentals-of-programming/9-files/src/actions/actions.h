#ifndef ACTIONS_H
#define ACTIONS_H
#include "../common/common.h"

// FILE ACTIONS
int actionCreateFile(FileContext *);
int actionReadFile(FileContext *);
int actionDeleteFile(FileContext *);

// RECORD ACTIONS
int actionCreateRecord(FileContext *);
int actionReadRecord(FileContext *);
int actionEditRecord(FileContext *);
int actionSortRecords(FileContext *);
int actionInsertRecordIfSorted(FileContext *);
int actionDeleteRecord(FileContext *);

// MISC ACTIONS
int actionDisplayMenu();
int actionExitProgram();
int actionNotImplemented(FileContext *context);
int actionVerifyFileHeader();

#endif
