#include "LexicalAnalyzerContext.h"
#include "../../shared/utils.h"
#include <stdlib.h>
#include <string.h>

/**
 * Flex exported variables and functions.
 *
 * @see https://westes.github.io/flex/manual/User-Values.html
 */

// The wrapper of "yy_top_state" function (provided by Flex).
extern unsigned int flexCurrentContext(void);

// The lexeme length in characters (provided by Flex).
extern const int yyleng;

// The line number (provided by Flex).
extern const int yylineno;

// The semantic value of the lookahead symbol (provided by Bison).
extern union SemanticValue yylval;

// The current lexeme (provided by Flex).
extern const char* const yytext;

/* PUBLIC FUNCTIONS */

LexicalAnalyzerContext* createLexicalAnalyzerContext() {
  LexicalAnalyzerContext* lexicalAnalyzerContext = safeMalloc(sizeof(LexicalAnalyzerContext));
  lexicalAnalyzerContext->length = yyleng;
  lexicalAnalyzerContext->lexeme = safeCalloc(1 + yyleng, sizeof(char));
  lexicalAnalyzerContext->line = yylineno;
  lexicalAnalyzerContext->semanticValue = &yylval;
  lexicalAnalyzerContext->currentContext = flexCurrentContext();
  strncpy(lexicalAnalyzerContext->lexeme, yytext, yyleng);
  return lexicalAnalyzerContext;
}

void destroyLexicalAnalyzerContext(LexicalAnalyzerContext* lexicalAnalyzerContext) {
  if (lexicalAnalyzerContext != NULL) {
    if (lexicalAnalyzerContext->lexeme != NULL) {
      free(lexicalAnalyzerContext->lexeme);
    }
    free(lexicalAnalyzerContext);
  }
}
