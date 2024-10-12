#include "BisonActions.h"
#include "../../shared/Array.h"
#include "../../shared/ArrayElement.h"
#include "../../shared/CompilerState.h"
#include "../../shared/Logger.h"
#include "../../shared/Set.h"
#include "../../shared/SetElement.h"
#include "../../shared/String.h"
#include "../../shared/utils.h"
#include "ASTUtils.h"
#include "AbstractSyntaxTree.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* MODULE INTERNAL STATE */

static Logger* _logger = NULL;

void initializeBisonActionsModule() {
  _logger = createLogger("BisonActions");
}

void shutdownBisonActionsModule() {
  if (_logger != NULL) {
    destroyLogger(_logger);
  }
}

/** IMPORTED FUNCTIONS */

extern unsigned int flexCurrentContext(void);

/* PRIVATE FUNCTIONS */

static void _logSyntacticAnalyzerAction(const char* functionName);

/**
 * Logs a syntactic-analyzer action in DEBUGGING level.
 */
static void _logSyntacticAnalyzerAction(const char* functionName) {
  logDebugging(_logger, "%s", functionName);
}

static void _logSyntacticAnalyzerPushAction(const char* functionName, const char* const format, ...) {
  va_list arguments;
  va_start(arguments, format);
  char* effectiveFormat = safeAsprintf("%s(%s)", functionName, format);
  vlogDebugging(_logger, effectiveFormat, arguments);
  free(effectiveFormat);
  va_end(arguments);
}

/* PUBLIC FUNCTIONS */

Program* ProgramSemanticAction(CompilerState* compilerState, SentenceArray sentences) {
  _logSyntacticAnalyzerAction(__func__);
  Program* program = safeMalloc(sizeof(Program));
  program->sentences = sentences;
  compilerState->abstractSyntaxtTree = program;

  if (0 < flexCurrentContext()) {
    logError(_logger, "The final context is not the default (0): %d", flexCurrentContext());
    compilerState->succeed = false;
  } else {
    compilerState->succeed = true;
  }

  return program;
}

#define INIT_CAP 10

SentenceArray SentenceArray_new(Sentence* sentence) {
  _logSyntacticAnalyzerAction(__func__);
  SentenceArray array = Array_new(INIT_CAP, SentenceArray_freeEle, NULL);
  SentenceArray_push(array, sentence);

  return array;
}

SentenceArray SentenceArray_push(SentenceArray array, Sentence* sentence) {
  char* str = Sentence_toString(sentence);
  _logSyntacticAnalyzerPushAction(__func__, str);
  free(str);
  ArrayElement ele = {.sentence = sentence};
  Array_push(array, ele);
  return array;
}

Sentence* GrammarDefinitionSentence_new(GrammarDefinition* grammarDefinition) {
  _logSyntacticAnalyzerAction(__func__);
  Sentence* sentence = safeMalloc(sizeof(Sentence));
  sentence->type = GRAMMAR_DEFINITION;
  sentence->grammarDefinition = grammarDefinition;

  return sentence;
}

Sentence* SymbolSetBindingSentence_new(SymbolSetBinding* symbolSetBinding) {
  _logSyntacticAnalyzerAction(__func__);
  Sentence* sentence = safeMalloc(sizeof(Sentence));
  sentence->type = SYMBOL_SET;
  sentence->symbolSetBinding = symbolSetBinding;

  return sentence;
}

Sentence* ProductionSetBindingSentence_new(ProductionSetBinding* productionSetBinding) {
  _logSyntacticAnalyzerAction(__func__);
  Sentence* sentence = safeMalloc(sizeof(Sentence));
  sentence->type = PRODUCTION_SET;
  sentence->productionSetBinding = productionSetBinding;

  return sentence;
}

GrammarDefinition*
GrammarDefinition_new(Id grammarId, Id terminalSetId, Id nonTerminalSetId, Id productionSetId, Id initialSymbolId) {
  _logSyntacticAnalyzerAction(__func__);
  GrammarDefinition* grammar = safeMalloc(sizeof(GrammarDefinition));
  grammar->id = grammarId;
  grammar->terminalSetId = terminalSetId;
  grammar->nonTerminalSetId = nonTerminalSetId;
  grammar->productionSetId = productionSetId;
  grammar->initialSymbolId = initialSymbolId;
  return grammar;
}

SymbolSetBinding* SymbolSetBinding_new(Id setId, SymbolSet symbols) {
  _logSyntacticAnalyzerAction(__func__);
  SymbolSetBinding* symbolSetBinding = safeMalloc(sizeof(SymbolSetBinding));
  symbolSetBinding->id = setId;
  symbolSetBinding->symbols = symbols;

  return symbolSetBinding;
}

SymbolSet SymbolSet_new(Symbol symbol) {
  _logSyntacticAnalyzerAction(__func__);
  SymbolSet set = Set_new(Symbol_hashEle, Symbol_equalsEle, Symbol_freeEle, Symbol_toStringEle);
  SymbolSet_add(set, symbol);

  return set;
}

SymbolSet SymbolSet_add(SymbolSet array, Symbol symbol) {
  _logSyntacticAnalyzerPushAction(__func__, "Symbol(%s)", symbol);
  SetElement ele = {.symbol = symbol};
  Set_add(array, ele);
  return array;
}

ProductionSetBinding* ProductionSetBinding_new(Id setId, ProductionSet productions) {
  _logSyntacticAnalyzerAction(__func__);
  ProductionSetBinding* productionSetBinding = safeMalloc(sizeof(ProductionSetBinding));
  productionSetBinding->id = setId;
  productionSetBinding->productions = productions;

  return productionSetBinding;
}

ProductionSet ProductionSet_new(Production* production) {
  _logSyntacticAnalyzerAction(__func__);
  ProductionSet set = Set_new(Production_hashEle, Production_equalsEle, Production_freeEle, Production_toStringEle);
  ProductionSet_add(set, production);

  return set;
}

ProductionSet ProductionSet_add(ProductionSet set, Production* production) {
  char* str = Production_toString(production);
  _logSyntacticAnalyzerPushAction(__func__, "Production(%s)", str);
  free(str);
  SetElement ele = {.production = production};
  SetElement* foundEle = Set_find(set, ele);
  if (foundEle == NULL) Set_add(set, ele);
  else {
    Set_union(foundEle->production->rhs, production->rhs);
    free(production->lhs.symbol);
    free(production);
  }
  return set;
}

Production* Production_new(Symbol lhs, ProductionRhsRuleSet productionRhsRules) {
  _logSyntacticAnalyzerAction(__func__);
  Production* production = safeMalloc(sizeof(Production));
  production->lhs = lhs;
  production->rhs = productionRhsRules;

  return production;
}

ProductionRhsRuleSet ProductionRhsRuleSet_new(ProductionRhsRule* productionRhsRule) {
  _logSyntacticAnalyzerAction(__func__);
  ProductionRhsRuleSet set = Set_new(
    ProductionRhsRule_hashEle, ProductionRhsRule_equalsEle, ProductionRhsRule_freeEle, ProductionRhsRule_toStringEle
  );
  ProductionRhsRuleSet_add(set, productionRhsRule);

  return set;
}

ProductionRhsRuleSet ProductionRhsRuleSet_add(ProductionRhsRuleSet set, ProductionRhsRule* rule) {
  char* str = ProductionRhsRule_toString(rule);
  _logSyntacticAnalyzerPushAction(__func__, "ProductionRhsRule%s", str);
  free(str);
  SetElement ele = {.productionRhsRule = rule};
  Set_add(set, ele);
  return set;
}

ProductionRhsRule* ProductionRhsRuleSymbolSymbol_new(Symbol leftSymbol, Symbol rightSymbol) {
  ProductionRhsRule* rule = safeMalloc(sizeof(ProductionRhsRule));
  rule->type = SYMBOL_SYMBOL_T;
  rule->leftSymbol = leftSymbol;
  rule->rightSymbol = rightSymbol;
  return rule;
}

ProductionRhsRule* ProductionRhsRuleSymbol_new(Symbol symbol) {
  ProductionRhsRule* rule = safeMalloc(sizeof(ProductionRhsRule));
  rule->type = SYMBOL_T;
  rule->symbol = symbol;
  return rule;
}

ProductionRhsRule* ProductionRhsRuleLambda_new() {
  ProductionRhsRule* rule = safeMalloc(sizeof(ProductionRhsRule));
  rule->type = LAMBDA_T;
  return rule;
}

///////////// Language operations /////////////
Language* Language_new(Id grammarId) {
  _logSyntacticAnalyzerAction(__FUNCTION__);
  Language* language = safeMalloc(sizeof(Language));
  language->grammarId = grammarId;
  return language;
}

LanguageExpression* SimpleLanguageExpression_new(Language* language) {
  _logSyntacticAnalyzerAction(__FUNCTION__);
  LanguageExpression* langExpression = safeMalloc(sizeof(LanguageExpression));
  langExpression->type = LANGUAGE;
  langExpression->language = language;
  return langExpression;
}

LanguageExpression* ComplexLanguageExpression_new(
  LanguageExpression* leftLang, LanguageExpression* rightlang, LanguageExpressionType type
) {
  _logSyntacticAnalyzerAction(__FUNCTION__);
  LanguageExpression* langExpression = safeMalloc(sizeof(LanguageExpression));
  langExpression->type = type;
  langExpression->leftLanguageExpression = leftLang;
  langExpression->rightLanguageExpression = rightlang;
  return langExpression;
}

LanguageBinding* LanguageBinding_new(Id langId, LanguageExpression* langExpression) {
  LanguageBinding* languageBinding = safeMalloc(sizeof(LanguageBinding));
  languageBinding->id = langId;
  languageBinding->LanguageExpression = langExpression;
  return languageBinding;
}

Sentence* LanguageBindingSentence_new(LanguageBinding* languageBinding) {
  _logSyntacticAnalyzerAction(__func__);
  Sentence* sentence = safeMalloc(sizeof(Sentence));
  sentence->type = LANGUAGE_SENTENCE;
  sentence->languageBinding = languageBinding;
  return sentence;
}
///////////// Set operations /////////////

SymbolSet SymbolSetUnion(SymbolSet left, SymbolSet right) {
  char* leftStr = Set_toString(left);
  char* rightStr = Set_toString(right);
  _logSyntacticAnalyzerPushAction(__func__, "SymbolSet(%s) ∪ SymbolSet(%s)", leftStr, rightStr);
  free(leftStr);
  free(rightStr);
  Set_union(left, right);
  return left;
}

ProductionSet ProductionSetUnion(ProductionSet left, ProductionSet right) {
  char* leftStr = Set_toString(left);
  char* rightStr = Set_toString(right);
  _logSyntacticAnalyzerPushAction(__func__, "ProductionSet(%s) ∪ ProductionSet(%s)", leftStr, rightStr);
  free(leftStr);
  free(rightStr);
  SetIterator rightIter = SetIterator_new(right);
  while (SetIterator_hasNext(rightIter)) {
    Production* prod = SetIterator_next(rightIter)->production;
    ProductionSet_add(left, prod);
  }
  SetIterator_free(rightIter);
  Set_freeNotElements(right);
  return left;
}
