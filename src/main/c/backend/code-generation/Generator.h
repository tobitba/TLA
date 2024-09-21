#ifndef GENERATOR_HEADER
#define GENERATOR_HEADER

#include "../../shared/CompilerState.h"
#include <stdarg.h>

/** Initialize module's internal state. */
void initializeGeneratorModule();

/** Shutdown module's internal state. */
void shutdownGeneratorModule();

/**
 * Generates the final output using the current compiler state.
 */
void generate(CompilerState* compilerState);

#endif
