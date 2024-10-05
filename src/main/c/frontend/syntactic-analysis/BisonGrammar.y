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
	Token token;
  Id id;
  Symbol symbol;

	/** Non-terminals. */
	Program* program;
  SentenceArray sentences;
  Sentence* sentence;
  GrammarDefinition* grammarDefinition;
  SymbolSetBinding* symbolSetBinding;
  SymbolArray symbolSet;
  SymbolArray symbols;
  ProductionSetBinding* productionSetBinding;
  ProductionArray productionSet;
  ProductionArray productions;
  Production* production;
  ProductionRhsRuleArray productionRhsRules;
  ProductionRhsRule* productionRhsRule;
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
%token <id> ID
%token <token> EQUALS
%token <token> ANGLE_BRACKET_OPEN
%token <token> ANGLE_BRACKET_CLOSE
%token <token> BRACES_OPEN
%token <token> BRACES_CLOSE
%token <token> COMMA
%token <token> RIGHT_ARROW
%token <token> LAMBDA
%token <token> PIPE
%token <token> UNION
%token <symbol> SYMBOL

%token <token> UNKNOWN

/** Non-terminals. */
%type <program> program
%type <sentences> sentences
%type <sentence> sentence
%type <grammarDefinition> grammarDefinition
%type <symbolSetBinding> symbolSetBinding
%type <symbolSet> symbolSet
%type <symbols> symbols
%type <productionSetBinding> productionSetBinding
%type <productionSet> productionSet
%type <productions> productions
%type <production> production
%type <productionRhsRules> productionRhsRules
%type <productionRhsRule> productionRhsRule

/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
%left UNION

%%

// IMPORTANT: To use λ in the following grammar, use the %empty symbol.

// ProgramSemanticAction should validate that all grammars are properly defined (all its sets exist and have no erros, etc).
program: sentences                              { $$ = ProgramSemanticAction(currentCompilerState(), $1); }

sentences: sentence                             { $$ = SentenceArray_new($1); }
  | sentences[list] sentence[val]               { $$ = SentenceArray_push($list, $val); }
  ;

sentence: grammarDefinition                     { $$ = GrammarDefinitionSentenceSemanticAction($1); }
  | symbolSetBinding                                   { $$ = SymbolSetBindingSentenceSemanticAction($1); }
  | productionSetBinding                               { $$ = ProductionSetBindingSentenceSemanticAction($1); }
  ;

grammarDefinition:
  ID[grammarId] EQUALS ANGLE_BRACKET_OPEN
    ID[terminalsId] COMMA
    ID[nonTerminalsId] COMMA
    ID[productionsId] COMMA
    ID[initialSymbolId]
  ANGLE_BRACKET_CLOSE                           { $$ = GrammarDefinitionSemanticAction(
                                                         $grammarId, $terminalsId, $nonTerminalsId, $productionsId, $initialSymbolId
                                                       );
                                                }

symbolSetBinding:
  ID[setId] EQUALS symbolSet[set]                               { $$ = SymbolSetBindingSemanticAction($setId, $set); }

symbolSet: BRACES_OPEN symbols[values] BRACES_CLOSE             { $$ = $values; }
  | symbolSet[left] UNION symbolSet[right]                      { $$ = SymbolSetUnion($left, $right); }
  ;

symbols: SYMBOL                                                 { $$ = SymbolArray_new($1); }
  | symbols[list] COMMA SYMBOL[val]                             { $$ = SymbolArray_push($list, $val); }
  ;

productionSetBinding:
  ID[setId] EQUALS productionSet[set]                           { $$ = ProductionSetBindingSemanticAction($setId, $set); }

productionSet: BRACES_OPEN productions[values] BRACES_CLOSE     { $$ = $values; }
  | productionSet[left] UNION productionSet[right]              { $$ = ProductionSetUnion($left, $right); }
  ;

productions: production                                         { $$ = ProductionArray_new($1); }
  | productions[list] COMMA production[val]                     { $$ = ProductionArray_push($list, $val); }
  ;

production: SYMBOL[lhs] RIGHT_ARROW productionRhsRules[rhs]     { $$ = ProductionSemanticAction($lhs, $rhs); }

productionRhsRules: productionRhsRule                           { $$ = ProductionRhsRuleArray_new($1); }
  | productionRhsRules[list] PIPE productionRhsRule[val]        { $$ = ProductionRhsRuleArray_push($list, $val); }
  ;

productionRhsRule: SYMBOL SYMBOL                                { $$ = ProductionRhsRuleSymbolSymbolSemanticAction($1, $2); }
  | SYMBOL                                                      { $$ = ProductionRhsRuleSymbolSemanticAction($1); }
  | LAMBDA                                                      { $$ = ProductionRhsRuleLambdaSemanticAction(); }
  ;

%%
