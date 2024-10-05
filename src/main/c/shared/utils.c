#include <errno.h>
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { CALLSTACK_LEN = 128, EXTRA_MSG_LEN = 20 };

void exitWithPerror(const char* functionName, const char* msg) {
  size_t functionNameLen = strlen(functionName);
  size_t msgLen = strlen(msg);
  size_t fullMsgLen = functionNameLen + msgLen + EXTRA_MSG_LEN;
  char fullMsg[fullMsgLen];
  strncpy(fullMsg, "@", fullMsgLen);
  size_t idx = strlcat(fullMsg + 1, functionName, fullMsgLen);
  idx = strlcat(fullMsg + idx, " ", fullMsgLen);
  strlcat(fullMsg + idx, msg, fullMsgLen);
  perror(fullMsg);
  printf("\n-----------------------------   Stack trace   -----------------------------\n\n");
  void* callstack[CALLSTACK_LEN];
  int frames = backtrace(callstack, CALLSTACK_LEN);
  char** strs = backtrace_symbols(callstack, frames);
  for (int i = 0; i < frames; ++i) {
    printf("%s\n", strs[i]);
  }
  free((void*)strs);
  return;
  exit(EXIT_FAILURE);
}

void exitWithErrno(int errnoVal, const char* functionName, const char* msg) {
  errno = errnoVal;
  exitWithPerror(functionName, msg);
}

void exitInvalidArgument(const char* functionName, const char* msg) {
  exitWithErrno(EINVAL, functionName, msg);
}

void* safeMalloc(size_t size) {
  void* ptr = malloc(size);
  if (ptr == NULL) exitWithPerror(__func__, "@malloc error");
  return ptr;
}

void* safeCalloc(size_t n, size_t size) {
  void* ptr = calloc(n, size);
  if (ptr == NULL) exitWithPerror(__func__, "@calloc error");
  return ptr;
}
