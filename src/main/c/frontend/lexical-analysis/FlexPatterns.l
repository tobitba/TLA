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
%x SET_DEFINITION

/**
 * Reusable patterns.
 *
 * @see https://westes.github.io/flex/manual/Matching.html
 * @see https://westes.github.io/flex/manual/Patterns.html
 */

id_r ([[:alpha:]]|_+[[:alpha:]])[[:alnum:]_]*
symbol_r {id_r}

%%

"/*"                                { BEGIN(MULTILINE_COMMENT); BeginMultilineCommentLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>"*/"             { EndMultilineCommentLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL); }
<MULTILINE_COMMENT>[^*]+            { IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>.                { IgnoredLexemeAction(createLexicalAnalyzerContext()); }

"//"                                { BEGIN(SINGLE_COMMENT); BeginSingleLineCommentLexemeAction(createLexicalAnalyzerContext()); }
<SINGLE_COMMENT>\n                  { EndSingleLineCommentLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL); }
<SINGLE_COMMENT>[^\n]+              { IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<SINGLE_COMMENT>.                   { IgnoredLexemeAction(createLexicalAnalyzerContext()); }

"<"                                 {
                                      BEGIN(GRAMMAR_DEFINITION);
                                      BeginGrammarDefinitionLexemeAction(createLexicalAnalyzerContext());
                                      return AngleBracketOpenLexemeAction(createLexicalAnalyzerContext());
                                    }
<GRAMMAR_DEFINITION>">"             { 
                                      EndGrammarDefinitionLexemeAction(createLexicalAnalyzerContext());
                                      Token t = AngleBracketOpenLexemeAction(createLexicalAnalyzerContext());
                                      BEGIN(INITIAL);
                                      return t;
                                    }
<GRAMMAR_DEFINITION>{id_r}          { return IdLexemeAction(createLexicalAnalyzerContext()); }
<GRAMMAR_DEFINITION>","             { return CommaLexemeAction(createLexicalAnalyzerContext()); }
<GRAMMAR_DEFINITION>[[:space:]]+    { IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<GRAMMAR_DEFINITION>.               { return UnknownLexemeAction(createLexicalAnalyzerContext()); }

"{"                                 { 
                                      BEGIN(GRAMMAR_DEFINITION);
                                      BeginSetDefinitionLexemeAction(createLexicalAnalyzerContext());
                                      return BracesOpenLexemeAction(createLexicalAnalyzerContext());
                                    }
<SET_DEFINITION>"}"                 { 
                                      EndSetDefinitionLexemeAction(createLexicalAnalyzerContext());
                                      Token t = BracesCloseLexemeAction(createLexicalAnalyzerContext());
                                      BEGIN(INITIAL);
                                      return t;
                                    }
<SET_DEFINITION>{symbol_r}          { return SymbolLexemeAction(createLexicalAnalyzerContext()); }
<SET_DEFINITION>"lambda"            { return LambdaLexemeAction(createLexicalAnalyzerContext()); }
<SET_DEFINITION>"|"                 { return PipeLexemeAction(createLexicalAnalyzerContext()); }
<SET_DEFINITION>"->"                { return RightArrowLexemeAction(createLexicalAnalyzerContext()); }
<SET_DEFINITION>","                 { return CommaLexemeAction(createLexicalAnalyzerContext()); }
<SET_DEFINITION>[[:space:]]+        { IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<SET_DEFINITION>.                   { return UnknownLexemeAction(createLexicalAnalyzerContext()); }

{id_r}                              { return IdLexemeAction(createLexicalAnalyzerContext()); }
"="                                 { return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), SUB); }

[[:space:]]+                        { IgnoredLexemeAction(createLexicalAnalyzerContext()); }
.                                   { return UnknownLexemeAction(createLexicalAnalyzerContext()); }

%%

#include "FlexExport.h"
