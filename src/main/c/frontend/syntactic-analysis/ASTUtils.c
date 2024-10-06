#include "ASTUtils.h"
#include "../../shared/Array.h"
#include "../../shared/ArrayTypes.h"
#include "../../shared/Logger.h"
#include "../../shared/String.h"
#include "AbstractSyntaxTree.h"
#include <stdarg.h>
#include <stdlib.h>

static Logger* _logger = NULL;

#define RESET_COLOR "\033[0m"
#define GREEN_COLOR "\033[0;32m"
#define BLUE_COLOR "\033[0;34m"

#define COLORIZE_SYMBOL(str) GREEN_COLOR str RESET_COLOR
#define COLORIZE_ID(str) BLUE_COLOR str RESET_COLOR

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
    SymbolSetBinding_free(ele.sentence->symbolSetBinding);
    break;
  case PRODUCTION_SET:
    ProductionSetBinding_free(ele.sentence->productionSetBinding);
    break;
  }
  free(ele.sentence);
}

void GrammarDefinition_free(GrammarDefinition* grammarDefinition) {
  logDebugging(_logger, "Executing destructor: %s", __func__);
  free(grammarDefinition->id);
  free(grammarDefinition->terminalSetId);
  free(grammarDefinition->nonTerminalSetId);
  free(grammarDefinition->productionSetId);
  free(grammarDefinition->initialSymbolId);
  free(grammarDefinition);
}

void SymbolSetBinding_free(SymbolSetBinding* symbolSetBinding) {
  logDebugging(_logger, "Executing destructor: %s", __func__);
  Array_free(symbolSetBinding->symbols);
  free(symbolSetBinding->id);
  free(symbolSetBinding);
}

void ProductionSetBinding_free(ProductionSetBinding* productionSetBinding) {
  logDebugging(_logger, "Executing destructor: %s", __func__);
  Array_free(productionSetBinding->productions);
  free(productionSetBinding->id);
  free(productionSetBinding);
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
  free(ele.productionRhsRule);
}

// String conversions

char* SymbolArrayElement_toString(ArrayElement ele) {
  char* str = safeAsprintf(COLORIZE_SYMBOL("%s"), ele.symbol);
  return str;
}

char* Production_toString(Production* production) {
  char* rhs = Array_toString(production->rhs);
  char* str = safeAsprintf(COLORIZE_SYMBOL("%s") " -> %s", production->lhs, rhs);
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
      "[ " COLORIZE_SYMBOL("%s") ", " COLORIZE_SYMBOL("%s") " ]", productionRhsRule->leftSymbol,
      productionRhsRule->rightSymbol
    );
    break;
  case SYMBOL_T:
    str = safeAsprintf("[ " COLORIZE_SYMBOL("%s") " ]", productionRhsRule->symbol);
    break;
  case LAMBDA_T:
    str = safeAsprintf("[ " COLORIZE_SYMBOL("󰘧") " ]");
    break;
  }
  return str;
}

char* ProductionRhsRuleArrayElement_toString(ArrayElement ele) {
  return ProductionRhsRule_toString(ele.productionRhsRule);
}

char* GrammarDefinition_toString(GrammarDefinition* grammarDefinition) {
  char* str = safeAsprintf(
    "GrammarDefinition{ id: " COLORIZE_ID("%s") ", terminalSetId: " COLORIZE_ID("%s"
    ) ", nonTerminalSetId: " COLORIZE_ID("%s") ", productionSetId: " COLORIZE_ID("%s"
    ) ", initialSymbolId: " COLORIZE_ID("%s") " }",
    grammarDefinition->id, grammarDefinition->terminalSetId, grammarDefinition->nonTerminalSetId,
    grammarDefinition->productionSetId, grammarDefinition->initialSymbolId
  );
  return str;
}

char* SymbolSetBinding_toString(SymbolSetBinding* symbolSetBinding) {
  char* symbols = Array_toString(symbolSetBinding->symbols);
  char* str = safeAsprintf("SymbolSetBinding{ id: " COLORIZE_ID("%s") ", symbols: %s }", symbolSetBinding->id, symbols);
  free(symbols);
  return str;
}

char* ProductionSetBinding_toString(ProductionSetBinding* productionSetBinding) {
  char* productions = Array_toString(productionSetBinding->productions);
  char* str = safeAsprintf(
    "ProductionSetBinding{ id: " COLORIZE_ID("%s") ", productions: %s }", productionSetBinding->id, productions
  );
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
    str = SymbolSetBinding_toString(sentence->symbolSetBinding);
    break;
  case PRODUCTION_SET:
    str = ProductionSetBinding_toString(sentence->productionSetBinding);
    break;
  }
  return str;
}
