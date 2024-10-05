#include "ASTUtils.h"
#include "../../shared/Array.h"
#include "../../shared/ArrayTypes.h"
#include "../../shared/Logger.h"
#include "../../shared/String.h"
#include "AbstractSyntaxTree.h"
#include <stdarg.h>
#include <stdlib.h>

static Logger* _logger = NULL;

#define SYMBOL_COLOR INFORMATION_COLOR

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

void ProductionArray_freeEle(ArrayElement ele) {
  logDebugging(_logger, "Executing destructor: %s", __func__);
  free(ele.production->lhs);
  Array_free(ele.production->rhs);
  free(ele.production);
}

void ProductionRhsRuleArray_freeEle(ArrayElement ele) {
  logDebugging(_logger, "Executing destructor: %s", __func__);
  switch (ele.productionRhsRule->type) {
  case SYMBOL_SYMBOL_T:
    free(ele.productionRhsRule->leftSymbol);
    free(ele.productionRhsRule->rightSymbol);
    break;
  case SYMBOL_T:
    free(ele.productionRhsRule->symbol);
    break;
  default:
    break;
  }
}

// String conversions

char* SymbolArrayElement_toString(ArrayElement ele) {
  char* str = safeAsprintf(SYMBOL_COLOR "%s" DEFAULT_COLOR, ele.symbol);
  return str;
}

char* Production_toString(Production* production) {
  char* rhs = Array_toString(production->rhs);
  char* str = safeAsprintf(SYMBOL_COLOR "%s" DEFAULT_COLOR " -> %s", production->lhs, rhs);
  free(rhs);
  return str;
}

char* ProductionArrayElement_toString(ArrayElement ele) {
  return Production_toString(ele.production);
}

char* ProductionRhsRule_toString(ProductionRhsRule* productionRhsRule) {
  char* str;
  switch (productionRhsRule->type) {
  case SYMBOL_SYMBOL_T:
    str = safeAsprintf(
      "[ " SYMBOL_COLOR "%s" DEFAULT_COLOR ", " SYMBOL_COLOR "%s" DEFAULT_COLOR " ]", productionRhsRule->leftSymbol,
      productionRhsRule->rightSymbol
    );
    break;
  case SYMBOL_T:
    str = safeAsprintf("[ " SYMBOL_COLOR "%s" DEFAULT_COLOR " ]", productionRhsRule->symbol);
    break;
  case LAMBDA_T:
    str = safeAsprintf("[ " SYMBOL_COLOR "󰘧" DEFAULT_COLOR " ]");
    break;
  }
  return str;
}

char* ProductionRhsRuleArrayElement_toString(ArrayElement ele) {
  return ProductionRhsRule_toString(ele.productionRhsRule);
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
