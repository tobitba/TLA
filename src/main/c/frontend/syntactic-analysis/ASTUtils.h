#ifndef AST_UTILS_H
#define AST_UTILS_H

#include "../../shared/ArrayTypes.h"
#include "AbstractSyntaxTree.h"

void initializeASTUtilsModule();
void shutdownASTUtilsModule();
void Program_free(Program* program);
void SentenceArray_freeEle(ArrayElement ele);
void GrammarDefinition_free(GrammarDefinition* grammarDefinition);
void SymbolSet_free(SymbolSet* symbolSet);
void ProductionSet_free(ProductionSet* productionSet);
void SymbolArray_freeEle(ArrayElement ele);
void String_freeEle(ArrayElement ele);
void ProductionArray_freeEle(ArrayElement ele);
void ProductionRhsArray_freeEle(ArrayElement ele);
char* SymbolArrayElement_toString(ArrayElement ele);
char* StringElemenArrayElement_toString(ArrayElement ele);
char* ProductionArrayElement_toString(ArrayElement ele);
char* StringArrayElement_toString(ArrayElement ele);
char* GrammarDefinition_toString(GrammarDefinition* grammarDefinition);
char* SymbolSet_toString(SymbolSet* symbolSet);
char* ProductionSet_toString(ProductionSet* productionSet);
char* Sentence_toString(Sentence* sentence);

#endif
