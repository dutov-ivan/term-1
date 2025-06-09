#include "../common/common.h"
#include "../io/utils.h"
#include "actions.h"
#include "utils.h"
#include <stdlib.h>

int actionNotImplemented(FileContext *context) {
  handleError("This action isn't implemented yet!");
  return SUCCESS;
}

int actionExitProgram() { return EXIT; }

int actionVerifyFileHeader() {
  char *filename = getValidFilename();
  if (!filename)
    return FAILURE;

  FileContext *tempContext = initFileContext();
  if (!tempContext)
    goto cleanup;

  if (!fileExists(filename)) {
    handleError("This file doesn't exist, can't check the file header.");
    goto cleanup;
  }

  if (fillFileContext(tempContext, filename, SIGNATURE) == FAILURE)
    goto cleanup;

  if (handleBadFileHeader(tempContext) == FAILURE)
    goto cleanup;

  closeFileContext(tempContext);
  free(tempContext);
  free(filename);
  showSuccess("File \"%s\" does contain file header", filename);
  return SUCCESS;

cleanup:
  free(filename);
  if (tempContext) {
    closeFileContext(tempContext);
    free(tempContext);
  }
  return FAILURE;
}

int actionDisplayMenu() { return DISPLAY_MENU; }
