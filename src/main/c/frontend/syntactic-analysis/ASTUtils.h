#ifndef AST_UTILS_H
#define AST_UTILS_H

#include "../../shared/ArrayElement.h"
#include "../../shared/SetElement.h"
#include "AbstractSyntaxTree.h"
#include <stdbool.h>
#include <stdint.h>

void initializeASTUtilsModule();
void shutdownASTUtilsModule();

//////////// Program ////////////////
void Program_free(Program* program);
//////////// Sentences //////////////
void SentenceArray_freeEle(ArrayElement ele);
char* Sentence_toString(Sentence* sentence);
//////////// Grammars ///////////////
void GrammarDefinition_free(GrammarDefinition* grammarDefinition);
char* GrammarDefinition_toString(GrammarDefinition* grammarDefinition);
//////////// Symbols ////////////////
uint32_t Symbol_hashEle(SetElement ele);
bool Symbol_equalsEle(SetElement ele1, SetElement ele2);
void Symbol_freeEle(SetElement ele);
char* Symbol_toStringEle(SetElement ele);

void SymbolSetBinding_free(SymbolSetBinding* symbolSetBinding);
char* SymbolSetBinding_toString(SymbolSetBinding* symbolSetBinding);
//////////// Productions ////////////
uint32_t Production_hashEle(SetElement ele);
bool Production_equalsEle(SetElement ele1, SetElement ele2);
void Production_freeEle(SetElement ele);
char* Production_toString(Production* production);
char* Production_toStringEle(SetElement ele);

void ProductionSetBinding_free(ProductionSetBinding* productionSetBinding);
char* ProductionSetBinding_toString(ProductionSetBinding* productionSetBinding);
//////////// Productions Rhs ////////////
uint32_t ProductionRhsRule_hashEle(SetElement ele);
bool ProductionRhsRule_equalsEle(SetElement ele1, SetElement ele2);
void ProductionRhsRule_freeEle(SetElement ele);
char* ProductionRhsRule_toString(ProductionRhsRule* productionRhsRule);
char* ProductionRhsRule_toStringEle(SetElement ele);

//////////// Languages ////////////
void LanguageBinding_free(LanguageBinding* languageBinding);
void LanguegeExpression_free(LanguageExpression* languageExpression);
void Language_free(Language* language);
char* LanguageBinding_toString(LanguageBinding* languageBinding);

#endif
