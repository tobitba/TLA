%{

// C file dependencies

#include "BisonActions.h"
#include "./SyntacticAnalyzer.h"
#include "./AbstractSyntaxTree.h"

%}

%code requires {

// Header file dependencies

#include "./AbstractSyntaxTree.h"
#include "../../shared/Type.h"

}

// You touch this, and you die.
%define api.value.union.name SemanticValue

%union {
	/** Terminals. */

	/** Non-terminals. */

	Program* program;
}

/**
 * Destructors. This functions are executed after the parsing ends, so if the
 * AST must be used in the following phases of the compiler you shouldn't used
 * this approach. To use this mechanism, the AST must be translated into
 * another structure.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Destructor-Decl.html
 */
/*
%destructor { releaseConstant($$); } <constant>
%destructor { releaseExpression($$); } <expression>
%destructor { releaseFactor($$); } <factor>
%destructor { releaseProgram($$); } <program>
*/

/** Terminals. */
%token <> ID
%token <> EQUALS_SIGN
%token <> ANGLE_BRACKET_OPEN
%token <> ANGLE_BRACKET_CLOSE
%token <> BRACES_OPEN
%token <> BRACES_CLOSE
%token <> COMMA
%token <> RIGHT_ARROW
%token <> TERMINAL
%token <> NON_TERMINAL
%token <> LAMBDA
%token <> PIPE

%token <token> UNKNOWN

/** Non-terminals. */
%type <> terminalsSetId
%type <> nonTerminalsSetId
%type <> productionsSetId
%type <> initialSymbolId
%type <> grammarId
%type <> grammarDefinition

%type <> terminalsSet
%type <> nonTerminalsSet
%type <> productionsSet
%type <> production
%type <> rhs
%type <> initialSymbol
%type <> grammar

%type <program> program

/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
%left ADD SUB
%left MUL DIV

%%

// IMPORTANT: To use λ in the following grammar, use the %empty symbol.

// ProgramSemanticAction should validate that all grammars are properly defined (all its sets exist and have no erros, etc).
program: sentences                              { $$ = ProgramSemanticAction(currentCompilerState(), $1); }

sentences: sentence                             {}
  | sentences sentence                          {}
  ;

sentence: grammarDefinition                     {}
  | symbolSet                                   {}
  | productionSet                               {}
  ;

grammarDefinition:
  ID[grammarId] EQUALS ANGLE_BRACKET_OPEN
    ID[terminalsId] COMMA
    ID[nonTerminalsId] COMMA
    ID[productionsId] COMMA
    SYMBOL[initialSymbol]
  ANGLE_BRACKET_CLOSE                           { $$ = GrammarSemanticAction($grammarId, $terminalsId, $nonTerminalsId, $initialSymbol); }

symbolSet:
  ID[setId] EQUALS BRACES_OPEN
    symbols[values]
  BRACES_CLOSE                                  { $$ = SymbolSetSemanticAction($setId, $values); }

symbols: SYMBOL                                 { $$ = initializeSimbolList($1); }
  | symbols[list] COMMA SYMBOL[val]             { pushSymbol($list, $val); $$ = $list; }
  ;

productionSet:
  ID[setId] EQUALS BRACES_OPEN
    productions[values]
  BRACES_CLOSE                                  { $$ = ProductionSetSemanticAction($setId, $values); }

productions: production                         { $$ = initializeProductionList($1); }
  | productions[list] COMMA production[val]     { pushProduction($list, $val); $$ = $list }
  ;

production: SYMBOL RIGHT_ARROW productionRhs    { $$ = ProductionSemanticAction($1, $2); }

productionRhs: string                           { $$ = initializeProductionRhsList($1); }
  | productionsRhs[list] PIPE string[val]       { pushProductionRhs($list, $val); $$ = $list }
  ;

string: SYMBOL                                  { $$ = initializeString($1); }
  | string[list] SYMBOL[val]                    { appendSymbol($list, $val); $$ = $list }
  ;

%%
