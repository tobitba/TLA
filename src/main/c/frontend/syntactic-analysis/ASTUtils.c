#include "ASTUtils.h"
#include "../../shared/Array.h"
#include "../../shared/ArrayElement.h"
#include "../../shared/Logger.h"
#include "../../shared/Set.h"
#include "../../shared/SetElement.h"
#include "../../shared/String.h"
#include "../../shared/hashUtils.h"
#include "AbstractSyntaxTree.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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

//////////// Program ////////////////

void Program_free(Program* program) {
  logDebugging(_logger, "Executing destructor: %s", __func__);
  Array_free(program->sentences);
  free(program);
}

//////////// Sentences //////////////

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
  case LANGUAGE_SENTENCE:
    LanguageBinding_free(ele.sentence->languageBinding);
  }
  free(ele.sentence);
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
  case LANGUAGE_SENTENCE:
    str = LanguageBinding_toString(sentence->languageBinding);
    break;
  }

  return str;
}

//////////// Grammars ///////////////

void GrammarDefinition_free(GrammarDefinition* grammarDefinition) {
  logDebugging(_logger, "Executing destructor: %s", __func__);
  free(grammarDefinition->id.id);
  free(grammarDefinition->terminalSetId.id);
  free(grammarDefinition->nonTerminalSetId.id);
  free(grammarDefinition->productionSetId.id);
  free(grammarDefinition->initialSymbolId.id);
  free(grammarDefinition);
}

char* GrammarDefinition_toString(GrammarDefinition* grammarDefinition) {
  char* str = safeAsprintf(
    "GrammarDefinition{ id: " COLORIZE_ID("%s") ", terminalSetId: " COLORIZE_ID("%s"
    ) ", nonTerminalSetId: " COLORIZE_ID("%s") ", productionSetId: " COLORIZE_ID("%s"
    ) ", initialSymbolId: " COLORIZE_ID("%s") " }",
    grammarDefinition->id.id, grammarDefinition->terminalSetId.id, grammarDefinition->nonTerminalSetId.id,
    grammarDefinition->productionSetId.id, grammarDefinition->initialSymbolId.id
  );
  return str;
}

//////////// Symbols ////////////////

uint32_t Symbol_hash(Symbol symbol) {
  return murmurHash3(1, symbol.symbol, symbol.length);
}

uint32_t Symbol_hashEle(SetElement ele) {
  return Symbol_hash(ele.symbol);
}

bool Symbol_equals(Symbol symbol1, Symbol symbol2) {
  return symbol1.length == symbol2.length && strcmp(symbol1.symbol, symbol2.symbol) == 0;
}

bool Symbol_equalsEle(SetElement ele1, SetElement ele2) {
  return Symbol_equals(ele1.symbol, ele2.symbol);
}

void Symbol_freeEle(SetElement ele) {
  logDebugging(_logger, "Executing destructor: %s", __func__);
  free(ele.symbol.symbol);
}

char* Symbol_toString(Symbol symbol) {
  char* str = safeAsprintf(COLORIZE_SYMBOL("%s"), symbol.symbol);
  return str;
}

char* Symbol_toStringEle(SetElement ele) {
  return Symbol_toString(ele.symbol);
}

void SymbolSetBinding_free(SymbolSetBinding* symbolSetBinding) {
  logDebugging(_logger, "Executing destructor: %s", __func__);
  Set_free(symbolSetBinding->symbols);
  free(symbolSetBinding->id.id);
  free(symbolSetBinding);
}

char* SymbolSetBinding_toString(SymbolSetBinding* symbolSetBinding) {
  char* symbols = Set_toString(symbolSetBinding->symbols);
  char* str =
    safeAsprintf("SymbolSetBinding{ id: " COLORIZE_ID("%s") ", symbols: %s }", symbolSetBinding->id.id, symbols);
  free(symbols);
  return str;
}

//////////// Production Rhs ////////////

uint32_t ProductionRhsRule_hash(ProductionRhsRule* rule) {
  if (rule == NULL) return 0;
  switch (rule->type) {
  case SYMBOL_SYMBOL_T:
    return murmurHash3(
      3, &rule->type, sizeof(ProductionRhsRuleType), rule->leftSymbol.symbol, rule->leftSymbol.length,
      rule->rightSymbol.symbol, rule->rightSymbol.length
    );
    break;
  case SYMBOL_T:
    return murmurHash3(2, &rule->type, sizeof(ProductionRhsRuleType), rule->symbol.symbol, rule->symbol.length);
    break;
  case LAMBDA_T:
    return murmurHash3(1, &rule->type, sizeof(ProductionRhsRuleType));
    break;
  }
}

uint32_t ProductionRhsRule_hashEle(SetElement ele) {
  return ProductionRhsRule_hash(ele.productionRhsRule);
}

bool ProductionRhsRule_equals(ProductionRhsRule* rule1, ProductionRhsRule* rule2) {
  if (rule1 == rule2) return true;
  if (rule1 == NULL || rule2 == NULL) return false;

  bool cmp = rule1->type == rule2->type;

  if (cmp) {
    switch (rule1->type) {
    case SYMBOL_SYMBOL_T:
      cmp = strcmp(rule1->leftSymbol.symbol, rule2->leftSymbol.symbol) == 0;
      if (cmp) cmp = strcmp(rule1->rightSymbol.symbol, rule2->rightSymbol.symbol) == 0;
      break;
    case SYMBOL_T:
      cmp = strcmp(rule1->symbol.symbol, rule2->symbol.symbol) == 0;
      break;
    case LAMBDA_T:
      break;
    }
  }
  return cmp;
}

bool ProductionRhsRule_equalsEle(SetElement ele1, SetElement ele2) {
  return ProductionRhsRule_equals(ele1.productionRhsRule, ele2.productionRhsRule);
}

void ProductionRhsRule_free(ProductionRhsRule* rule) {
  logDebugging(_logger, "Executing destructor: %s", __func__);
  switch (rule->type) {
  case SYMBOL_SYMBOL_T:
    logInformation(_logger, "Case SS T %s  %s", rule->leftSymbol.symbol, rule->rightSymbol.symbol);
    free(rule->leftSymbol.symbol);
    free(rule->rightSymbol.symbol);
    break;
  case SYMBOL_T:
    logInformation(_logger, "Case S T %s", rule->symbol.symbol);
    free(rule->symbol.symbol);
    break;
  case LAMBDA_T:
    logInformation(_logger, "Freed Rule is lambda");
    break;
  }
  free(rule);
}

void ProductionRhsRule_freeEle(SetElement ele) {
  ProductionRhsRule_free(ele.productionRhsRule);
}

char* ProductionRhsRule_toString(ProductionRhsRule* productionRhsRule) {
  char* str;
  switch (productionRhsRule->type) {
  case SYMBOL_SYMBOL_T:
    str = safeAsprintf(
      "[ " COLORIZE_SYMBOL("%s") ", " COLORIZE_SYMBOL("%s") " ]", productionRhsRule->leftSymbol.symbol,
      productionRhsRule->rightSymbol
    );
    break;
  case SYMBOL_T:
    str = safeAsprintf("[ " COLORIZE_SYMBOL("%s") " ]", productionRhsRule->symbol.symbol);
    break;
  case LAMBDA_T:
    str = safeAsprintf("[ " COLORIZE_SYMBOL("󰘧") " ]");
    break;
  }
  return str;
}

char* ProductionRhsRule_toStringEle(SetElement ele) {
  return ProductionRhsRule_toString(ele.productionRhsRule);
}

//////////// Productions ////////////

uint32_t Production_hash(Production* production) {
  if (production == NULL) return 0;
  return murmurHash3(1, production->lhs.symbol, production->lhs.length);
}

uint32_t Production_hashEle(SetElement ele) {
  return Production_hash(ele.production);
}

bool Production_equals(Production* prod1, Production* prod2) {
  if (prod1 == prod2) return true;
  if (prod1 == NULL || prod2 == NULL) return false;
  return strcmp(prod1->lhs.symbol, prod2->lhs.symbol) == 0;
}

bool Production_equalsEle(SetElement ele1, SetElement ele2) {
  return Production_equals(ele1.production, ele2.production);
}

void Production_freeEle(SetElement ele) {
  logDebugging(_logger, "Executing destructor: %s", __func__);
  free(ele.production->lhs.symbol);
  Set_free(ele.production->rhs);
  free(ele.production);
}

char* Production_toString(Production* production) {
  char* rhs = Set_toString(production->rhs);
  char* str = safeAsprintf(COLORIZE_SYMBOL("%s") " -> %s", production->lhs.symbol, rhs);
  free(rhs);
  return str;
}

char* Production_toStringEle(SetElement ele) {
  return Production_toString(ele.production);
}

void ProductionSetBinding_free(ProductionSetBinding* productionSetBinding) {
  logDebugging(_logger, "Executing destructor: %s", __func__);
  Set_free(productionSetBinding->productions);
  free(productionSetBinding->id.id);
  free(productionSetBinding);
}

char* ProductionSetBinding_toString(ProductionSetBinding* productionSetBinding) {
  char* productions = Set_toString(productionSetBinding->productions);
  char* str = safeAsprintf(
    "ProductionSetBinding{ id: " COLORIZE_ID("%s") ", productions: %s }", productionSetBinding->id.id, productions
  );
  free(productions);
  return str;
}

//////////// LANGUAGES ////////////
void LanguageBinding_free(LanguageBinding* languageBinding) {
  logDebugging(_logger, "Executing destructor: %s", __func__);
  LanguageExpression_free(languageBinding->LanguageExpression);
  free(languageBinding->id.id);
  free(languageBinding);
}

void LanguageExpression_free(LanguageExpression* languageExpression) {
  logDebugging(_logger, "Executing destructor: %s", __func__);
  switch (languageExpression->type) {
  case LANGUAGE:
    Language_free(languageExpression->language);
    break;
  case LANG_UNION:
  case LANG_INTERSEC:
  case LANG_MINUS:
  case LANG_CONCAT:
    LanguageExpression_free(languageExpression->leftLanguageExpression);
    LanguageExpression_free(languageExpression->rightLanguageExpression);
    break;
  case LANG_REVERSE:
  case LANG_COMPLEMENT:
    LanguageExpression_free(languageExpression->unaryLanguageExpression);
    break;
  default:
    break;
  }
  free(languageExpression);
}

void Language_free(Language* language) {
  free(language->id.id);
  free(language);
}

// LANG_UNION, LANG_INTERSEC, LANG_MINUS, LANG_CONCAT, LANG_REVERSE
char LanguageExpressionType_toString(LanguageExpressionType type) {
  switch (type) {
  case LANG_UNION:
    return 'u';
  case LANG_INTERSEC:
    return 'n';
  case LANG_CONCAT:
    return '.';
  case LANG_MINUS:
    return '-';
  case LANG_REVERSE:
    return 'R';
  case LANG_COMPLEMENT:
    return 'N';
  default:
    return '?';
  }
}

char* LanguageExpression_toString(LanguageExpression* languageExpression) {
  if (languageExpression->type == LANGUAGE) {
    if (languageExpression->language->type == GRAMMAR_ID) {
      return safeAsprintf("L(" COLORIZE_ID("%s") ")", languageExpression->language->id);
    }
    return safeAsprintf("(" COLORIZE_ID("%s") ")", languageExpression->language->id);
  }
  if (languageExpression->type == LANG_COMPLEMENT || languageExpression->type == LANG_REVERSE) {
    char* unaryExpression = LanguageExpression_toString(languageExpression->unaryLanguageExpression);
    char languageExpressionType = LanguageExpressionType_toString(languageExpression->type);
    char* str = safeAsprintf("%c(%s)", languageExpressionType, unaryExpression);
    free(unaryExpression);
    return str;
  }

  char* leftExpression = LanguageExpression_toString(languageExpression->leftLanguageExpression);
  char* rightExpression = LanguageExpression_toString(languageExpression->rightLanguageExpression);
  char languageExpressionType = LanguageExpressionType_toString(languageExpression->type);
  char* str = safeAsprintf("%s %c %s", leftExpression, languageExpressionType, rightExpression);
  free(leftExpression);
  free(rightExpression);
  return str;
}

char* LanguageBinding_toString(LanguageBinding* languageBinding) {
  char* languageExpression = LanguageExpression_toString(languageBinding->LanguageExpression);
  char* str = safeAsprintf(
    "LanguageBinding{ id: " COLORIZE_ID("%s") ", languageExpression: %s }", languageBinding->id.id, languageExpression
  );
  free(languageExpression);
  return str;
}
