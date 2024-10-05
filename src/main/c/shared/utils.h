#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

__attribute__((noreturn)) void exitWithPerror(const char* functionName, const char* msg);
__attribute__((noreturn)) void exitWithErrno(int errnoVal, const char* functionName, const char* msg);
__attribute__((noreturn)) void exitInvalidArgument(const char* functionName, const char* msg);

void* safeMalloc(size_t size);
void* safeCalloc(size_t n, size_t size);

#endif
