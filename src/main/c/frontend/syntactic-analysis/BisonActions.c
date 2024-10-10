#include "BisonActions.h"
#include "../../shared/Array.h"
#include "../../shared/ArrayElement.h"
#include "../../shared/CompilerState.h"
#include "../../shared/Logger.h"
#include "../../shared/String.h"
#include "../../shared/Type.h"
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

Sentence* GrammarDefinitionSentenceSemanticAction(GrammarDefinition* grammarDefinition) {
  _logSyntacticAnalyzerAction(__func__);
  Sentence* sentence = safeMalloc(sizeof(Sentence));
  sentence->type = GRAMMAR_DEFINITION;
  sentence->grammarDefinition = grammarDefinition;

  return sentence;
}

Sentence* SymbolSetBindingSentenceSemanticAction(SymbolSetBinding* symbolSetBinding) {
  _logSyntacticAnalyzerAction(__func__);
  Sentence* sentence = safeMalloc(sizeof(Sentence));
  sentence->type = SYMBOL_SET;
  sentence->symbolSetBinding = symbolSetBinding;

  return sentence;
}

Sentence* ProductionSetBindingSentenceSemanticAction(ProductionSetBinding* productionSetBinding) {
  _logSyntacticAnalyzerAction(__func__);
  Sentence* sentence = safeMalloc(sizeof(Sentence));
  sentence->type = PRODUCTION_SET;
  sentence->productionSetBinding = productionSetBinding;

  return sentence;
}

GrammarDefinition* GrammarDefinitionSemanticAction(
  Id grammarId, Id terminalSetId, Id nonTerminalSetId, Id productionSetId, Id initialSymbolId
) {
  _logSyntacticAnalyzerAction(__func__);
  GrammarDefinition* grammar = safeMalloc(sizeof(GrammarDefinition));
  grammar->id = grammarId;
  grammar->terminalSetId = terminalSetId;
  grammar->nonTerminalSetId = nonTerminalSetId;
  grammar->productionSetId = productionSetId;
  grammar->initialSymbolId = initialSymbolId;
  return grammar;
}

SymbolSetBinding* SymbolSetBindingSemanticAction(Id setId, SymbolArray symbols) {
  _logSyntacticAnalyzerAction(__func__);
  SymbolSetBinding* symbolSetBinding = safeMalloc(sizeof(SymbolSetBinding));
  symbolSetBinding->id = setId;
  symbolSetBinding->symbols = symbols;

  return symbolSetBinding;
}

SymbolArray SymbolArray_new(Symbol symbol) {
  _logSyntacticAnalyzerAction(__func__);
  SymbolArray array = Array_new(INIT_CAP, SymbolArray_freeEle, SymbolArrayElement_toString);
  SymbolArray_push(array, symbol);

  return array;
}

SymbolArray SymbolArray_push(SymbolArray array, Symbol symbol) {
  _logSyntacticAnalyzerPushAction(__func__, "Symbol(%s)", symbol);
  ArrayElement ele = {.symbol = symbol};
  Array_push(array, ele);
  return array;
}

ProductionSetBinding* ProductionSetBindingSemanticAction(Id setId, ProductionArray productions) {
  _logSyntacticAnalyzerAction(__func__);
  ProductionSetBinding* productionSetBinding = safeMalloc(sizeof(ProductionSetBinding));
  productionSetBinding->id = setId;
  productionSetBinding->productions = productions;

  return productionSetBinding;
}

ProductionArray ProductionArray_new(Production* production) {
  _logSyntacticAnalyzerAction(__func__);
  SymbolArray array = Array_new(INIT_CAP, ProductionArray_freeEle, ProductionArrayElement_toString);
  ProductionArray_push(array, production);

  return array;
}

ProductionArray ProductionArray_push(ProductionArray array, Production* production) {
  char* str = Production_toString(production);
  _logSyntacticAnalyzerPushAction(__func__, "Production(%s)", str);
  free(str);
  ArrayElement ele = {.production = production};
  Array_push(array, ele);
  return array;
}

Production* ProductionSemanticAction(Symbol lhs, ProductionRhsRuleArray productionRhsRules) {
  _logSyntacticAnalyzerAction(__func__);
  Production* production = safeMalloc(sizeof(Production));
  production->lhs = lhs;
  production->rhs = productionRhsRules;

  return production;
}

ProductionRhsRuleArray ProductionRhsRuleArray_new(ProductionRhsRule* productionRhsRule) {
  _logSyntacticAnalyzerAction(__func__);
  ProductionRhsRuleArray array =
    Array_new(INIT_CAP, ProductionRhsRuleArray_freeEle, ProductionRhsRuleArrayElement_toString);
  ProductionRhsRuleArray_push(array, productionRhsRule);

  return array;
}

ProductionRhsRuleArray ProductionRhsRuleArray_push(ProductionRhsRuleArray array, ProductionRhsRule* productionRhsRule) {
  char* str = ProductionRhsRule_toString(productionRhsRule);
  _logSyntacticAnalyzerPushAction(__func__, "ProductionRhsRule(%s)", str);
  free(str);
  ArrayElement ele = {.productionRhsRule = productionRhsRule};
  Array_push(array, ele);
  return array;
}

ProductionRhsRule* ProductionRhsRuleSymbolSymbolSemanticAction(Symbol leftSymbol, Symbol rightSymbol) {
  ProductionRhsRule* rule = safeMalloc(sizeof(ProductionRhsRule));
  rule->type = SYMBOL_SYMBOL_T;
  rule->leftSymbol = leftSymbol;
  rule->rightSymbol = rightSymbol;
  return rule;
}

ProductionRhsRule* ProductionRhsRuleSymbolSemanticAction(Symbol symbol) {
  ProductionRhsRule* rule = safeMalloc(sizeof(ProductionRhsRule));
  rule->type = SYMBOL_T;
  rule->symbol = symbol;
  return rule;
}

ProductionRhsRule* ProductionRhsRuleLambdaSemanticAction() {
  ProductionRhsRule* rule = safeMalloc(sizeof(ProductionRhsRule));
  rule->type = LAMBDA_T;
  return rule;
}

///////////// Set operations /////////////

SymbolArray SymbolSetUnion(SymbolArray left, SymbolArray right) {
  char* leftStr = Array_toString(left);
  char* rightStr = Array_toString(right);
  _logSyntacticAnalyzerPushAction(__func__, "SymbolArray(%s) ∪ SymbolArray(%s)", leftStr, rightStr);
  free(leftStr);
  free(rightStr);
  Array_concat(left, right);
  return left;
}

ProductionArray ProductionSetUnion(ProductionArray left, ProductionArray right) {
  char* leftStr = Array_toString(left);
  char* rightStr = Array_toString(right);
  _logSyntacticAnalyzerPushAction(__func__, "ProductionArray(%s) ∪ ProductionArray(%s)", leftStr, rightStr);
  free(leftStr);
  free(rightStr);
  Array_concat(left, right);
  return left;
}
