#ifndef UTILS_H
#define UTILS_H

__attribute__((noreturn)) void exitWithPerror(const char* functionName, const char* msg);
__attribute__((noreturn)) void exitWithErrno(int errnoVal, const char* functionName, const char* msg);
__attribute__((noreturn)) void exitInvalidArgument(const char* functionName, const char* msg);

#endif
