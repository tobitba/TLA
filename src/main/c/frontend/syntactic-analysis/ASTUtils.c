#include "ASTUtils.h"
#include "../../shared/Array.h"
#include "../../shared/ArrayTypes.h"
#include "../../shared/Logger.h"
#include "../../shared/String.h"
#include "AbstractSyntaxTree.h"
#include <stdarg.h>
#include <stdlib.h>

static Logger* _logger = NULL;

void initializeASTUtilsModule() {
  _logger = createLogger("ASTUtils");
}

void shutdownASTUtilsModule() {
  if (_logger != NULL) {
    destroyLogger(_logger);
  }
}

// Memory deallocation

void Program_free(Program* program) {
  logDebugging(_logger, "Executing destructor: %s", __func__);
  Array_free(program->sentences);
  free(program);
}

void SentenceArray_freeEle(ArrayElement ele) {
  logDebugging(_logger, "Executing destructor: %s", __func__);
  switch (ele.sentence->type) {
  case GRAMMAR_DEFINITION:
    GrammarDefinition_free(ele.sentence->grammarDefinition);
    break;
  case SYMBOL_SET:
    SymbolSet_free(ele.sentence->symbolSet);
    break;
  case PRODUCTION_SET:
    ProductionSet_free(ele.sentence->productionSet);
    break;
  }
  free(ele.sentence);
}

void GrammarDefinition_free(GrammarDefinition* grammarDefinition) {
  logDebugging(_logger, "Executing destructor: %s", __func__);
  free(grammarDefinition->id);
  free(grammarDefinition->terminalSetId);
  free(grammarDefinition->nonTerminalSetId);
  free(grammarDefinition->initialSymbolId);
  free(grammarDefinition);
}

void SymbolSet_free(SymbolSet* symbolSet) {
  logDebugging(_logger, "Executing destructor: %s", __func__);
  Array_free(symbolSet->symbols);
  free(symbolSet);
}

void ProductionSet_free(ProductionSet* productionSet) {
  logDebugging(_logger, "Executing destructor: %s", __func__);
  Array_free(productionSet->productions);
  free(productionSet);
}

void SymbolArray_freeEle(ArrayElement ele) {
  logDebugging(_logger, "Executing destructor: %s", __func__);
  free(ele.symbol);
}

void String_freeEle(ArrayElement ele) {
  logDebugging(_logger, "Executing destructor: %s", __func__);
  if (ele.stringElement.type == SYMBOL_T) free(ele.symbol);
}

void ProductionArray_freeEle(ArrayElement ele) {
  logDebugging(_logger, "Executing destructor: %s", __func__);
  free(ele.production->lhs);
  Array_free(ele.production->rhs);
  free(ele.production);
}

void ProductionRhsArray_freeEle(ArrayElement ele) {
  logDebugging(_logger, "Executing destructor: %s", __func__);
  Array_free(ele.string);
}

// String conversions

char* SymbolArrayElement_toString(ArrayElement ele) {
  char* str = safeAsprintf("%s", ele.symbol);
  return str;
}

char* StringElemenArrayElement_toString(ArrayElement ele) {
  char* str = safeAsprintf("%s", ele.stringElement.type == SYMBOL_T ? ele.stringElement.symbol : "󰘧");
  return str;
}

char* ProductionArrayElement_toString(ArrayElement ele) {
  char* rhs = Array_toString(ele.production->rhs);
  char* str = safeAsprintf("%s -> %s", ele.production->lhs, rhs);
  free(rhs);
  return str;
}

char* StringArrayElement_toString(ArrayElement ele) {
  return Array_toString(ele.string);
}

char* GrammarDefinition_toString(GrammarDefinition* grammarDefinition) {
  char* str = safeAsprintf(
    "GrammarDefinition{ id: %s, terminalSetId: %s, nonTerminalSetId: %s, productionSetId: %s, initialSymbolId: %s }",
    grammarDefinition->id, grammarDefinition->terminalSetId, grammarDefinition->nonTerminalSetId,
    grammarDefinition->productionSetId, grammarDefinition->initialSymbolId
  );
  return str;
}

char* SymbolSet_toString(SymbolSet* symbolSet) {
  char* symbols = Array_toString(symbolSet->symbols);
  char* str = safeAsprintf("SymbolSet{ id: %s, symbols: %s }", symbolSet->id, symbols);
  free(symbols);
  return str;
}

char* ProductionSet_toString(ProductionSet* productionSet) {
  char* productions = Array_toString(productionSet->productions);
  char* str = safeAsprintf("ProductionSet{ id: %s, productions: %s }", productionSet->id, productions);
  free(productions);
  return str;
}

char* Sentence_toString(Sentence* sentence) {
  char* str;
  switch (sentence->type) {
  case GRAMMAR_DEFINITION:
    str = GrammarDefinition_toString(sentence->grammarDefinition);
    break;
  case SYMBOL_SET:
    str = SymbolSet_toString(sentence->symbolSet);
    break;
  case PRODUCTION_SET:
    str = ProductionSet_toString(sentence->productionSet);
    break;
  }
  return str;
}
