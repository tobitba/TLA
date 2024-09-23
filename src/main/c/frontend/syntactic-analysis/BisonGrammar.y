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

program: grammar                                                      {}
	;

grammar: terminalsSet nonTerminalsSet productionsSet initialSymbol    {}
  ;

terminalsSet:
  terminalsSetId EQUALS_SIGN BRACES_OPEN
    terminalsSetValues
  BRACES_CLOSE                              {}
  ;

terminalsSetId: ID                          {}
  ;

terminalsSetValues: TERMINAL                {}
  | terminalsSetValues COMMA TERMINAL       {}
  ;

nonTerminalsSet:
  nonTerminalsSetId EQUALS_SIGN BRACES_OPEN
    nonTerminalsSetValues
  BRACES_CLOSE                              {}
  ;

nonTerminalsSetValues: NON_TERMINAL             {}
  | nonTerminalsSetValues COMMA NON_TERMINAL    {}
  ;

productionsSet:
  productionsSetId EQUALS_SIGN BRACES_OPEN
    productionsSetValues
  BRACES_CLOSE                              {}
  ;

productionsSetValues: production            {}
  | productionsSetValues COMMA production   {}
  ;

// initialSymbol???
production: NON_TERMINAL RIGHT_ARROW rhs    {}
  | production PIPE rhs                     {}
  ;

rhs: LAMBDA                                 {}
  | TERMINAL                                {}
  | TERMINAL NON_TERMINAL                   {}
  ;

//// How to do this ////
grammarDefinition:
  grammarId EQUALS_SIGN ANGLE_BRACKET_OPEN
    terminalsSetId COMMA nonTerminalsSetId COMMA productionsSetId COMMA initialSymbolId
  ANGLE_BRACKET_CLOSE                       {}
  ;

grammarId: ID {}
  ;
terminalsSetId: ID {}
  ;
nonTerminalsSetId: ID {}
  ;
productionsSetId: ID {}
  ;
initialSymbolId: ID {}
  ;

# This doens't work because each consecutive id gets consumed to generate de next one...
# grammarId: ID[id] EQUALS_SIGN ANGLE_BRACKET_OPEN  {$$ = $id}
#   ;
# terminalsSetId: grammarId ID[id]                  {$$ = $id}
#   ;
# nonTerminalsSetId: terminalsSetId COMMA ID[id]    {$$ = $id}
#   ;
# productionsSetId: nonTerminalsSetId COMMA ID[id]  {$$ = $id}
#   ;
# initialSymbolId: productionsSetId COMMA ID[id]    {$$ = $id}
#   ;
////////////////////////

%%
