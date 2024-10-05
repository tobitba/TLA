#include "BisonActions.h"
#include "../../shared/Array.h"
#include "../../shared/ArrayTypes.h"
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

Sentence* SymbolSetSentenceSemanticAction(SymbolSet* symbolSet) {
  _logSyntacticAnalyzerAction(__func__);
  Sentence* sentence = safeMalloc(sizeof(Sentence));
  sentence->type = SYMBOL_SET;
  sentence->symbolSet = symbolSet;

  return sentence;
}

Sentence* ProductionSetSentenceSemanticAction(ProductionSet* productionSet) {
  _logSyntacticAnalyzerAction(__func__);
  Sentence* sentence = safeMalloc(sizeof(Sentence));
  sentence->type = PRODUCTION_SET;
  sentence->productionSet = productionSet;

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

SymbolSet* SymbolSetSemanticAction(Id setId, SymbolArray symbols) {
  _logSyntacticAnalyzerAction(__func__);
  SymbolSet* symbolSet = safeMalloc(sizeof(SymbolSet));
  symbolSet->id = setId;
  symbolSet->symbols = symbols;

  return symbolSet;
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

ProductionSet* ProductionSetSemanticAction(Id setId, ProductionArray productions) {
  _logSyntacticAnalyzerAction(__func__);
  ProductionSet* productionSet = safeMalloc(sizeof(ProductionSet));
  productionSet->id = setId;
  productionSet->productions = productions;

  return productionSet;
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
  SymbolArray array = Array_new(INIT_CAP, ProductionRhsRuleArray_freeEle, ProductionRhsRuleArrayElement_toString);
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
