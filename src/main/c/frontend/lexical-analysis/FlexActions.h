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
void IgnoredLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext);

Token ArithmeticOperatorLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext, Token token);
Token IntegerLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext);
Token ParenthesisLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext, Token token);

Token UnknownLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext);

#endif
