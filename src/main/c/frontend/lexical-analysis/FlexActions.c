#include "FlexActions.h"
#include "../../shared/Environment.h"
#include "../../shared/Logger.h"
#include "../../shared/String.h"
#include "../../shared/Type.h"
#include "../syntactic-analysis/BisonParser.h"
#include "LexicalAnalyzerContext.h"
#include <stdio.h>
#include <stdlib.h>

/* MODULE INTERNAL STATE */

static Logger* _logger = NULL;
static boolean _logIgnoredLexemes = true;

void initializeFlexActionsModule() {
  _logIgnoredLexemes = getBooleanOrDefault("LOG_IGNORED_LEXEMES", _logIgnoredLexemes);
  _logger = createLogger("FlexActions");
}

void shutdownFlexActionsModule() {
  if (_logger != NULL) {
    destroyLogger(_logger);
  }
}

/* PRIVATE FUNCTIONS */

static void _logLexicalAnalyzerContext(const char* functionName, LexicalAnalyzerContext* lexicalAnalyzerContext);

/**
 * Logs a lexical-analyzer context in DEBUGGING level.
 */
static void _logLexicalAnalyzerContext(const char* functionName, LexicalAnalyzerContext* lexicalAnalyzerContext) {
  char* escapedLexeme = escape(lexicalAnalyzerContext->lexeme);
  logDebugging(
    _logger, "%s: %s (context = %d, length = %d, line = %d)", functionName, escapedLexeme,
    lexicalAnalyzerContext->currentContext, lexicalAnalyzerContext->length, lexicalAnalyzerContext->line
  );
  free(escapedLexeme);
}

void _logIgnoredLexemeAction(const char* functionName, LexicalAnalyzerContext* lexicalAnalyzerContext) {
  if (_logIgnoredLexemes) {
    _logLexicalAnalyzerContext(functionName, lexicalAnalyzerContext);
  }
}

/* PUBLIC FUNCTIONS */

void BeginMultilineCommentLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext) {
  _logIgnoredLexemeAction(__FUNCTION__, lexicalAnalyzerContext);
}

void EndMultilineCommentLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext) {
  _logIgnoredLexemeAction(__FUNCTION__, lexicalAnalyzerContext);
}

void IgnoredLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext) {
  _logIgnoredLexemeAction(__FUNCTION__, lexicalAnalyzerContext);
}

Token ArithmeticOperatorLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext, Token token) {
  _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
  lexicalAnalyzerContext->semanticValue->token = token;
  return token;
}

Token IntegerLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext) {
  _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
  lexicalAnalyzerContext->semanticValue->integer = atoi(lexicalAnalyzerContext->lexeme);
  return INTEGER;
}

Token ParenthesisLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext, Token token) {
  _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
  lexicalAnalyzerContext->semanticValue->token = token;
  return token;
}

Token UnknownLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext) {
  _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
  return UNKNOWN;
}
