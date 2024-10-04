#include "AbstractSyntaxTree.h"
#include "../../shared/Logger.h"
#include "ASTUtils.h"
#include <stddef.h>
#include <stdlib.h>

/* MODULE INTERNAL STATE */

static Logger* _logger = NULL;

void initializeAbstractSyntaxTreeModule() {
  _logger = createLogger("AbstractSyntxTree");
}

void shutdownAbstractSyntaxTreeModule() {
  if (_logger != NULL) {
    destroyLogger(_logger);
  }
}

/** PUBLIC FUNCTIONS */

void releaseProgram(Program* program) {
  logDebugging(_logger, "Executing destructor: %s", __func__);
  if (program != NULL) {
    Program_free(program);
  }
}
