#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../shared/CompilerState.h"
#include "AbstractSyntaxTree.h"

/** Initialize module's internal state. */
void initializeBisonActionsModule();

/** Shutdown module's internal state. */
void shutdownBisonActionsModule();

/**
 * Bison semantic actions.
 */

Program* ProgramSemanticAction(CompilerState* compilerState, SentenceArray sentences);
SentenceArray SentenceArray_new(Sentence* sentence);
SentenceArray SentenceArray_push(SentenceArray array, Sentence* sentence);
Sentence* GrammarDefinitionSentence_new(GrammarDefinition* grammarDefinition);
Sentence* SymbolSetBindingSentence_new(SymbolSetBinding* symbolSetBinding);
Sentence* ProductionSetBindingSentence_new(ProductionSetBinding* productionSetBinding);

GrammarDefinition*
GrammarDefinition_new(Id grammarId, Id terminalSetId, Id nonTerminalSetId, Id productionSetId, Id initialSymbolId);

SymbolSetBinding* SymbolSetBinding_new(Id setId, SymbolSet symbols);

SymbolSet SymbolSet_new(Symbol symbol);
SymbolSet SymbolSet_add(SymbolSet array, Symbol symbol);

ProductionSetBinding* ProductionSetBinding_new(Id setId, ProductionSet productions);

ProductionSet ProductionSet_new(Production* production);
ProductionSet ProductionSet_add(ProductionSet set, Production* production);

Production* Production_new(Symbol lhs, ProductionRhsRuleSet productionRhsRules);

ProductionRhsRuleSet ProductionRhsRuleSet_new(ProductionRhsRule* productionRhsRule);
ProductionRhsRuleSet ProductionRhsRuleSet_add(ProductionRhsRuleSet set, ProductionRhsRule* rule);
ProductionRhsRule* ProductionRhsRuleSymbolSymbol_new(Symbol leftSymbol, Symbol rightSymbol);
ProductionRhsRule* ProductionRhsRuleSymbol_new(Symbol symbol);
ProductionRhsRule* ProductionRhsRuleLambda_new();

Language* Language_new(Id grammarId);
LanguageExpression* SimpleLanguageExpression_new(Language* language);
LanguageExpression* UnaryTypeLanguageExpression_new(LanguageExpression* lang, LanguageExpressionType type);
LanguageExpression* ComplexLanguageExpression_new(LanguageExpression* leftLang, LanguageExpression* rightLang, LanguageExpressionType type);
LanguageBinding* LanguageBinding_new(Id langId, LanguageExpression* langExpression);
Sentence* LanguageBindingSentence_new(LanguageBinding* languageBinding);

// Set operations
SymbolSet SymbolSetUnion(SymbolSet left, SymbolSet right);
ProductionSet ProductionSetUnion(ProductionSet left, ProductionSet right);

#endif
