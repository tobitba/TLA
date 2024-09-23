%{

#include <stdint.h>
#include "FlexActions.h"
#include "LexicalAnalyzerContext.h"
#include "../syntactic-analysis/BisonParser.h"

%}

/**
 * Enable start condition manipulation functions.
 */
%option stack

/**
 * Flex contexts (a.k.a. start conditions).
 *
 * @see https://westes.github.io/flex/manual/Start-Conditions.html
 */
%x MULTILINE_COMMENT
%x SINGLE_COMMENT
%x GRAMMAR_DEFINITION

/**
 * Reusable patterns.
 *
 * @see https://westes.github.io/flex/manual/Matching.html
 * @see https://westes.github.io/flex/manual/Patterns.html
 */

%%

"/*"								                { BEGIN(MULTILINE_COMMENT); BeginMultilineCommentLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>"*/"			      	{ EndMultilineCommentLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL); }
<MULTILINE_COMMENT>[^*]+            { IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>.                { IgnoredLexemeAction(createLexicalAnalyzerContext()); }

"//"								                { BEGIN(SINGLE_COMMENT); BeginSingleLineCommentLexemeAction(createLexicalAnalyzerContext()); }
<SINGLE_COMMENT>"\n"  			      	{ EndSingleLineCommentLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL); }
<SINGLE_COMMENT>[^\n]+              { IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<SINGLE_COMMENT>.                   { IgnoredLexemeAction(createLexicalAnalyzerContext()); }

"<"								                  { BEGIN(GRAMMAR_DEFINITION); BeginGrammarDefinitionLexemeAction(createLexicalAnalyzerContext()); }
<GRAMMAR_DEFINITION>">"  			      { EndGrammarDefinitionLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL); }
<GRAMMAR_DEFINITION>[[:space:]]*[[:alpha:]][[:alnum:]]*[[:space:]]* {
                                      return VariableNameLexemeAction(createLexicalAnalyzerContext());
                                    }
<GRAMMAR_DEFINITION>.               { IgnoredLexemeAction(createLexicalAnalyzerContext()); }

"-"									                { return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), SUB); }
"*"									                { return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), MUL); }
"/"									                { return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), DIV); }
"+"									                { return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), ADD); }

"("									                { return ParenthesisLexemeAction(createLexicalAnalyzerContext(), OPEN_PARENTHESIS); }
")"									                { return ParenthesisLexemeAction(createLexicalAnalyzerContext(), CLOSE_PARENTHESIS); }

[[:digit:]]+                        { return IntegerLexemeAction(createLexicalAnalyzerContext()); }

[[:space:]]+                        { IgnoredLexemeAction(createLexicalAnalyzerContext()); }
.                 									{ return UnknownLexemeAction(createLexicalAnalyzerContext()); }

%%

#include "FlexExport.h"
