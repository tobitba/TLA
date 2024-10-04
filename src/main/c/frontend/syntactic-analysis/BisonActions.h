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
Sentence* SymbolSetSentenceSemanticAction(SymbolSet* symbolSet);
Sentence* ProductionSetSentenceSemanticAction(ProductionSet* productionSet);
GrammarDefinition* GrammarDefinitionSemanticAction(
  Id grammarId, Id terminalSetId, Id nonTerminalSetId, Id productionSetId, Id initialSymbolId
);
SymbolSet* SymbolSetSemanticAction(Id setId, SymbolArray symbols);
SymbolArray SymbolArray_new(Symbol symbol);
SymbolArray SymbolArray_push(SymbolArray array, Symbol symbol);
ProductionSet* ProductionSetSemanticAction(Id setId, ProductionArray productions);
ProductionArray ProductionArray_new(Production* production);
ProductionArray ProductionArray_push(ProductionArray array, Production* production);
Production* ProductionSemanticAction(Symbol lhs, ProductionRhsArray strings);
ProductionRhsArray ProductionRhsArray_new(SymbolArray string);
ProductionRhsArray ProductionRhsArray_push(ProductionRhsArray array, SymbolArray string);
String String_new();
String String_pushLambda(String array);
String String_pushSymbol(String array, Symbol symbol);

#endif
