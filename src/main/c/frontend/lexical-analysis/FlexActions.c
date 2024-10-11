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
  _logIgnoredLexemeAction(__func__, lexicalAnalyzerContext);
  destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
}

void EndMultilineCommentLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext) {
  _logIgnoredLexemeAction(__func__, lexicalAnalyzerContext);
  destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
}

void BeginSingleLineCommentLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext) {
  _logIgnoredLexemeAction(__func__, lexicalAnalyzerContext);
  destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
}

void EndSingleLineCommentLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext) {
  _logIgnoredLexemeAction(__func__, lexicalAnalyzerContext);
  destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
}

void IgnoredLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext) {
  _logIgnoredLexemeAction(__func__, lexicalAnalyzerContext);
  destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
}

void BeginGrammarDefinitionLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext) {
  _logLexicalAnalyzerContext(__func__, lexicalAnalyzerContext);
  destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
}

void EndGrammarDefinitionLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext) {
  _logLexicalAnalyzerContext(__func__, lexicalAnalyzerContext);
  destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
}

void BeginSetDefinitionLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext) {
  _logLexicalAnalyzerContext(__func__, lexicalAnalyzerContext);
  destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
}

void EndSetDefinitionLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext) {
  _logLexicalAnalyzerContext(__func__, lexicalAnalyzerContext);
  destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
}

Token TokenLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext, Token token) {
  _logLexicalAnalyzerContext(__func__, lexicalAnalyzerContext);
  lexicalAnalyzerContext->semanticValue->token = token;
  destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
  return token;
}

Token IdLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext) {
  _logLexicalAnalyzerContext(__func__, lexicalAnalyzerContext);
  lexicalAnalyzerContext->semanticValue->id.id = lexicalAnalyzerContext->lexeme;
  lexicalAnalyzerContext->semanticValue->id.length = lexicalAnalyzerContext->length;
  free(lexicalAnalyzerContext);
  return ID;
}

Token SymbolLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext) {
  _logLexicalAnalyzerContext(__func__, lexicalAnalyzerContext);
  lexicalAnalyzerContext->semanticValue->symbol.symbol = lexicalAnalyzerContext->lexeme;
  lexicalAnalyzerContext->semanticValue->symbol.length = lexicalAnalyzerContext->length;
  free(lexicalAnalyzerContext);
  return SYMBOL;
}

Token UnknownLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext) {
  _logLexicalAnalyzerContext(__func__, lexicalAnalyzerContext);
  destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
  return UNKNOWN;
}
