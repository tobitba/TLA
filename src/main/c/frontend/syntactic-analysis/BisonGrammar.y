%{

// C file dependencies

#include "BisonActions.h"
#include "./SyntacticAnalyzer.h"
#include "./AbstractSyntaxTree.h"
#include "../../shared/Array.h"
#include <stdio.h>

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
  SymbolSet symbolSet;
  SymbolSet symbols;
  ProductionSetBinding* productionSetBinding;
  ProductionSet productionSet;
  ProductionSet productions;
  Production* production;
  ProductionRhsRuleSet productionRhsRules;
  ProductionRhsRule* productionRhsRule;
  LanguageBinding* languageBinding;
  LanguageExpression* languageExpression;
  Language* language;
}

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
%token <token> INTERSECTION
%token <token> SUBTRACTION
%token <token> CONCAT
%token <token> COMPLEMENT
%token <token> LANG_REVERSE_PARENTHESIS_OPEN
%token <symbol> SYMBOL
%token <token> L
%token <token> PARENTHESIS_OPEN
%token <token> PARENTHESIS_CLOSE

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
%type <languageBinding> languageBinding
%type <languageExpression> languageExpression
%type <language> language

/**
 * Destructors. This functions are executed after the parsing ends, so if the
 * AST must be used in the following phases of the compiler you shouldn't used
 * this approach. To use this mechanism, the AST must be translated into
 * another structure.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Destructor-Decl.html
 */
/*

From the documentation and my own testing these actually seem to only be executed when an error occurs, so they're
actually needed.

%destructor { printf("\nFreeing id\n\n"); free($$); } ID
%destructor { printf("\nFreeing SymbolSet\n\n"); Array_free($$); } symbols
%destructor { printf("\nFreeing ProductionSet\n\n"); Array_free($$); } symbols
%destructor { printf("\nFreeing GrammarDefinition\n\n"); GrammarDefinition_free($$); } grammarDefinition
%destructor { printf("\nFreeing SymbolSetBinding\n\n"); SymbolSetBinding_free($$); } symbolSetBinding
%destructor { printf("\nFreeing ProductionSetBinding\n\n"); ProductionSetBinding_free($$); } symbolSetBinding
*/

/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
%left UNION INTERSECTION SUBTRACTION CONCAT
%left COMPLEMENT


%%

// IMPORTANT: To use λ in the following grammar, use the %empty symbol.

// ProgramSemanticAction should validate that all grammars are properly defined (all its sets exist and have no erros, etc).
program: sentences                              { $$ = ProgramSemanticAction(currentCompilerState(), $1); }

sentences: sentence                             { $$ = SentenceArray_new($1); }
  | sentences[list] sentence[val]               { $$ = SentenceArray_push($list, $val); }
  ;

sentence: grammarDefinition                     { $$ = GrammarDefinitionSentence_new($1); }
  | symbolSetBinding                            { $$ = SymbolSetBindingSentence_new($1); }
  | productionSetBinding                        { $$ = ProductionSetBindingSentence_new($1); }
  | languageBinding                             { $$ = LanguageBindingSentence_new($1); }
  ;

grammarDefinition:
  ID[grammarId] EQUALS ANGLE_BRACKET_OPEN
    ID[terminalsId] COMMA
    ID[nonTerminalsId] COMMA
    ID[productionsId] COMMA
    ID[initialSymbolId]
  ANGLE_BRACKET_CLOSE                           { $$ = GrammarDefinition_new(
                                                         $grammarId, $terminalsId, $nonTerminalsId, $productionsId, $initialSymbolId
                                                       );
                                                }

symbolSetBinding:
  ID[setId] EQUALS symbolSet[set]                               { $$ = SymbolSetBinding_new($setId, $set); }

symbolSet: BRACES_OPEN symbols[values] BRACES_CLOSE             { $$ = $values; }
  | BRACES_OPEN symbols[values] COMMA BRACES_CLOSE              { $$ = $values; }
  | symbolSet[left] UNION symbolSet[right]                      { $$ = SymbolSetUnion($left, $right); }
  ;

symbols: SYMBOL                                                 { $$ = SymbolSet_new($1); }
  | symbols[list] COMMA SYMBOL[val]                             { $$ = SymbolSet_add($list, $val); }
  ;

productionSetBinding:
  ID[setId] EQUALS productionSet[set]                           { $$ = ProductionSetBinding_new($setId, $set); }

productionSet: BRACES_OPEN productions[values] BRACES_CLOSE     { $$ = $values; }
  | BRACES_OPEN productions[values] COMMA BRACES_CLOSE          { $$ = $values; }
  | productionSet[left] UNION productionSet[right]              { $$ = ProductionSetUnion($left, $right); }
  ;

productions: production                                         { $$ = ProductionSet_new($1); }
  | productions[list] COMMA production[val]                     { $$ = ProductionSet_add($list, $val); }
  ;

production: SYMBOL[lhs] RIGHT_ARROW productionRhsRules[rhs]     { $$ = Production_new($lhs, $rhs); }

productionRhsRules: productionRhsRule                           { $$ = ProductionRhsRuleSet_new($1); }
  | productionRhsRules[list] PIPE productionRhsRule[val]        { $$ = ProductionRhsRuleSet_add($list, $val); }
  ;

productionRhsRule: SYMBOL SYMBOL                                { $$ = ProductionRhsRuleSymbolSymbol_new($1, $2); }
  | SYMBOL                                                      { $$ = ProductionRhsRuleSymbol_new($1); }
  | LAMBDA                                                      { $$ = ProductionRhsRuleLambda_new(); }
  ;

languageBinding: ID[languageID] EQUALS languageExpression[lang]             { $$ = LanguageBinding_new($languageID, $lang); } 

languageExpression: language                                                { $$ = SimpleLanguageExpression_new($1); }
 | languageExpression[left] UNION languageExpression[right]                 { $$ = ComplexLanguageExpression_new($left, $right, LANG_UNION); }
 | languageExpression[left] INTERSECTION languageExpression[right]          { $$ = ComplexLanguageExpression_new($left, $right, LANG_INTERSEC); }
 | languageExpression[left] SUBTRACTION languageExpression[right]           { $$ = ComplexLanguageExpression_new($left, $right, LANG_MINUS); }
 | languageExpression[left] CONCAT languageExpression[right]                { $$ = ComplexLanguageExpression_new($left, $right, LANG_CONCAT); }
 | LANG_REVERSE_PARENTHESIS_OPEN languageExpression[lang] PARENTHESIS_CLOSE { $$ = UnaryTypeLanguageExpression_new($lang, LANG_REVERSE); }
 | COMPLEMENT languageExpression[lang]                                      { $$ = UnaryTypeLanguageExpression_new($lang, LANG_COMPLEMENT); }
 | PARENTHESIS_OPEN languageExpression[lang] PARENTHESIS_CLOSE              { $$ = $lang; }
 ;

language: L PARENTHESIS_OPEN ID[grammarID] PARENTHESIS_CLOSE              { $$ = Language_new($grammarID); }

%%
