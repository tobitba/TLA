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
%x SINGLE_LINE_COMMENT
%x GRAMMAR_DEFINITION
%x SET_DEFINITION

/**
 * Reusable patterns.
 *
 * @see https://westes.github.io/flex/manual/Matching.html
 * @see https://westes.github.io/flex/manual/Patterns.html
 */

id_r ([[:alpha:]]|_+[[:alnum:]])[[:alnum:]_]*
symbol_r [^[:space:],{}]+

%%

"="                                 { return TokenLexemeAction(createLexicalAnalyzerContext(), EQUALS); }
"u"                                 { return TokenLexemeAction(createLexicalAnalyzerContext(), UNION); }
"n"                                 { return TokenLexemeAction(createLexicalAnalyzerContext(), INTERSECTION); }
"-"                                 { return TokenLexemeAction(createLexicalAnalyzerContext(), SUBTRACTION); }
{id_r}                              { return IdLexemeAction(createLexicalAnalyzerContext()); }

"/*"                                { BEGIN(MULTILINE_COMMENT); BeginMultilineCommentLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>"*/"             { EndMultilineCommentLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL); }
%{
// Note: I'm pretty sure this rule isn't strictly necessary as `.` will already ignore everything but `*/` will still have
// precedence (flex is greedy) so the comment will be closed without problem. It may be more efficient to have this rule though,
// as without it flex executes `IgnoredLexemeAction` for each individual character, instead of chunks of text that don't contain `*`.
%}
<MULTILINE_COMMENT>[^*]+            { IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>.                { IgnoredLexemeAction(createLexicalAnalyzerContext()); }

"//"                                { BEGIN(SINGLE_LINE_COMMENT); BeginSingleLineCommentLexemeAction(createLexicalAnalyzerContext()); }
<SINGLE_LINE_COMMENT>\n             { EndSingleLineCommentLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL); }
<SINGLE_LINE_COMMENT>[^\n]+         { IgnoredLexemeAction(createLexicalAnalyzerContext()); }

"<"                                 {
                                      BEGIN(GRAMMAR_DEFINITION);
                                      BeginGrammarDefinitionLexemeAction(createLexicalAnalyzerContext());
                                      return TokenLexemeAction(createLexicalAnalyzerContext(), ANGLE_BRACKET_OPEN);
                                    }
<GRAMMAR_DEFINITION>">"             { 
                                      EndGrammarDefinitionLexemeAction(createLexicalAnalyzerContext());
                                      Token t = TokenLexemeAction(createLexicalAnalyzerContext(), ANGLE_BRACKET_CLOSE);
                                      BEGIN(INITIAL);
                                      return t;
                                    }
<GRAMMAR_DEFINITION>{id_r}          { return IdLexemeAction(createLexicalAnalyzerContext()); }
<GRAMMAR_DEFINITION>","             { return TokenLexemeAction(createLexicalAnalyzerContext(), COMMA); }
<GRAMMAR_DEFINITION>[[:space:]]+    { IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<GRAMMAR_DEFINITION>.               { return UnknownLexemeAction(createLexicalAnalyzerContext()); }

"{"                                 { 
                                      BEGIN(SET_DEFINITION);
                                      BeginSetDefinitionLexemeAction(createLexicalAnalyzerContext());
                                      return TokenLexemeAction(createLexicalAnalyzerContext(), BRACES_OPEN);
                                    }
<SET_DEFINITION>"}"                 { 
                                      EndSetDefinitionLexemeAction(createLexicalAnalyzerContext());
                                      Token t = TokenLexemeAction(createLexicalAnalyzerContext(), BRACES_CLOSE);
                                      BEGIN(INITIAL);
                                      return t;
                                    }
%{
// Note: The `{symbol_r}` rule matches the same strings as the reserved tokens, so it has to go last
// so that the reserved tokens have precedence.
%}
<SET_DEFINITION>"lambda"            { return TokenLexemeAction(createLexicalAnalyzerContext(), LAMBDA); }
<SET_DEFINITION>"|"                 { return TokenLexemeAction(createLexicalAnalyzerContext(), PIPE); }
<SET_DEFINITION>"->"                { return TokenLexemeAction(createLexicalAnalyzerContext(), RIGHT_ARROW); }
<SET_DEFINITION>","                 { return TokenLexemeAction(createLexicalAnalyzerContext(), COMMA); }
<SET_DEFINITION>{symbol_r}          { return SymbolLexemeAction(createLexicalAnalyzerContext()); }
<SET_DEFINITION>[[:space:]]+        { IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<SET_DEFINITION>.                   { return UnknownLexemeAction(createLexicalAnalyzerContext()); }

[[:space:]]+                        { IgnoredLexemeAction(createLexicalAnalyzerContext()); }
.                                   { return UnknownLexemeAction(createLexicalAnalyzerContext()); }

%%

#include "FlexExport.h"
