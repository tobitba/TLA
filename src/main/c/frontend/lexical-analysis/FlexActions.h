#ifndef FLEX_ACTIONS_HEADER
#define FLEX_ACTIONS_HEADER

#include "../../shared/Type.h"
#include "LexicalAnalyzerContext.h"

/** Initialize module's internal state. */
void initializeFlexActionsModule();

/** Shutdown module's internal state. */
void shutdownFlexActionsModule();

/**
 * Flex lexeme processing actions.
 */

void BeginMultilineCommentLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext);
void EndMultilineCommentLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext);
void BeginSingleLineCommentLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext);
void EndSingleLineCommentLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext);
void IgnoredLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext);

void BeginGrammarDefinitionLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext);
void EndGrammarDefinitionLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext);
void BeginSetDefinitionLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext);
void EndSetDefinitionLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext);

Token TokenLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext, Token token);
Token IdLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext);
Token SymbolLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext);

Token UnknownLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext);

#endif
