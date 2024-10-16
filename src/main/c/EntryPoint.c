// #include "backend/code-generation/Generator.h"
// #include "backend/domain-specific/Calculator.h"
#include "frontend/lexical-analysis/FlexActions.h"
#include "frontend/syntactic-analysis/ASTUtils.h"
#include "frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "frontend/syntactic-analysis/BisonActions.h"
#include "frontend/syntactic-analysis/SyntacticAnalyzer.h"
#include "shared/Array.h"
#include "shared/CompilerState.h"
#include "shared/Logger.h"
#include "shared/Set.h"
#include "shared/Type.h"
#include <stddef.h>
#include <stdlib.h>

/**
 * The main entry-point of the entire application. If you use "strtok" to
 * parse anything inside this project instead of using Flex and Bison, I will
 * find you, and I will kill you (Bryan Mills; "Taken", 2008).
 */
int main(const int count, const char** arguments) {
  Logger* logger = createLogger("EntryPoint");
  initializeFlexActionsModule();
  initializeBisonActionsModule();
  initializeSyntacticAnalyzerModule();
  initializeAbstractSyntaxTreeModule();
  // initializeCalculatorModule();
  // initializeGeneratorModule();
  Array_initializeLogger();
  Set_initializeLogger();
  initializeASTUtilsModule();

  // Logs the arguments of the application.
  for (int k = 0; k < count; ++k) {
    logDebugging(logger, "Argument %d: \"%s\"", k, arguments[k]);
  }

  // Begin compilation process.
  CompilerState compilerState = {.abstractSyntaxtTree = NULL, .succeed = false, .value = 0};
  const SyntacticAnalysisStatus syntacticAnalysisStatus = parse(&compilerState);
  CompilationStatus compilationStatus = SUCCEED;
  if (syntacticAnalysisStatus == ACCEPT) {
    // ----------------------------------------------------------------------------------------
    // Beginning of the Backend... ------------------------------------------------------------
    logInformation(logger, "Program:");
    Program* program = compilerState.abstractSyntaxtTree;

    size_t sentencesLen = Array_getLen(program->sentences);
    for (int i = 0; i < sentencesLen; ++i) {
      Sentence* sentence = Array_get(program->sentences, i).sentence;
      char* sentenceStr = Sentence_toString(sentence);
      logInformation(logger, "Sentence %d: %s", i, sentenceStr);
      free(sentenceStr);
    }

    // ComputationResult computationResult = computeExpression(program->expression);
    // if (computationResult.succeed) {
    //   compilerState.value = computationResult.value;
    //   generate(&compilerState);
    // } else {
    //   logError(logger, "The computation phase rejects the input program.");
    //   compilationStatus = FAILED;
    // }
    // ...end of the Backend. -----------------------------------------------------------------
    // ----------------------------------------------------------------------------------------
    logDebugging(logger, "Releasing AST resources...");
    releaseProgram(program);
  } else {
    logError(logger, "The syntactic-analysis phase rejects the input program.");
    compilationStatus = FAILED;
  }

  logDebugging(logger, "Releasing modules resources...");
  Array_freeLogger();
  Set_freeLogger();
  shutdownASTUtilsModule();
  // shutdownGeneratorModule();
  // shutdownCalculatorModule();
  shutdownAbstractSyntaxTreeModule();
  shutdownSyntacticAnalyzerModule();
  shutdownBisonActionsModule();
  shutdownFlexActionsModule();
  logDebugging(logger, "Compilation is done.");
  destroyLogger(logger);
  return compilationStatus;
}
