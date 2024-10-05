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
Sentence* GrammarDefinitionSentenceSemanticAction(GrammarDefinition* grammarDefinition);
Sentence* SymbolSetBindingSentenceSemanticAction(SymbolSetBinding* symbolSetBinding);
Sentence* ProductionSetBindingSentenceSemanticAction(ProductionSetBinding* productionSetBinding);
GrammarDefinition* GrammarDefinitionSemanticAction(
  Id grammarId, Id terminalSetId, Id nonTerminalSetId, Id productionSetId, Id initialSymbolId
);
SymbolSetBinding* SymbolSetBindingSemanticAction(Id setId, SymbolArray symbols);
SymbolArray SymbolArray_new(Symbol symbol);
SymbolArray SymbolArray_push(SymbolArray array, Symbol symbol);
ProductionSetBinding* ProductionSetBindingSemanticAction(Id setId, ProductionArray productions);
ProductionArray ProductionArray_new(Production* production);
ProductionArray ProductionArray_push(ProductionArray array, Production* production);
Production* ProductionSemanticAction(Symbol lhs, ProductionRhsRuleArray productionRhsRules);
ProductionRhsRuleArray ProductionRhsRuleArray_new(ProductionRhsRule* productionRhsRule);
ProductionRhsRuleArray ProductionRhsRuleArray_push(ProductionRhsRuleArray array, ProductionRhsRule* productionRhsRule);
ProductionRhsRule* ProductionRhsRuleSymbolSymbolSemanticAction(Symbol leftSymbol, Symbol rightSymbol);
ProductionRhsRule* ProductionRhsRuleSymbolSemanticAction(Symbol symbol);
ProductionRhsRule* ProductionRhsRuleLambdaSemanticAction();

// Set operations
SymbolArray SymbolSetUnion(SymbolArray left, SymbolArray right);
ProductionArray ProductionSetUnion(ProductionArray left, ProductionArray right);

#endif
