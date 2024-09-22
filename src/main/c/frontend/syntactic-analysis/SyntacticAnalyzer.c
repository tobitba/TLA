#include "SyntacticAnalyzer.h"
#include "../../shared/CompilerState.h"
#include "../../shared/Logger.h"
#include "../../shared/Type.h"
#include "../lexical-analysis/LexicalAnalyzerContext.h"
#include <stddef.h>

/* MODULE INTERNAL STATE */

static CompilerState* _currentCompilerState = NULL;
static Logger* _logger = NULL;

void initializeSyntacticAnalyzerModule() {
  _logger = createLogger("SyntacticAnalyzer");
}

void shutdownSyntacticAnalyzerModule() {
  if (_logger != NULL) {
    destroyLogger(_logger);
  }
}

/** IMPORTED FUNCTIONS */

/**
 * Bison exported functions.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Error-Reporting-Function.html
 */

/**
 * Bison main entry-point.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Parser-Function.html
 */
extern int yyparse(void);

// Bison error-reporting function.
void yyerror(const char* error) {
  LexicalAnalyzerContext* lexicalAnalyzerContext = createLexicalAnalyzerContext();
  logError(_logger, "%s (on line %d).", error, lexicalAnalyzerContext->line);
}

/* PUBLIC FUNCTIONS */

CompilerState* currentCompilerState() {
  return _currentCompilerState;
}

SyntacticAnalysisStatus parse(CompilerState* compilerState) {
  logDebugging(_logger, "Parsing...");
  _currentCompilerState = compilerState;
  const int code = yyparse();
  _currentCompilerState = NULL;
  SyntacticAnalysisStatus syntacticAnalysisStatus;
  logDebugging(_logger, "Parsing is done.");
  switch (code) {
  case 0:
    if (compilerState->succeed == true) {
      return ACCEPT;
    } else {
      syntacticAnalysisStatus = REJECT;
    }
    break;
  case 1:
    syntacticAnalysisStatus = REJECT;
    break;
  case 2:
    logError(_logger, "Bison ran out of memory.");
    syntacticAnalysisStatus = OUT_OF_MEMORY;
    break;
  default:
    logError(_logger, "Unknown error inside Bison engine (code = %d).", code);
    syntacticAnalysisStatus = UNKNOWN_ERROR;
  }
  compilerState->succeed = false;
  return syntacticAnalysisStatus;
}
