#include "Logger.h"
#include "Environment.h"
#include "String.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* PRIVATE FUNCTIONS */

static void _log(const Logger* logger, const LoggingLevel loggingLevel, const char* const format, va_list arguments);
static LoggingLevel _loggingLevelFromString(const char* loggingLevel);
static void _logInStream(FILE* const stream, const char* const format, va_list arguments);
static const char* _toContextString(const LoggingLevel loggingLevel);

/**
 * Logs a new message at the specified level, using a format string.
 */
static void _log(const Logger* logger, const LoggingLevel loggingLevel, const char* const format, va_list arguments) {
  if (logger->loggingLevel <= loggingLevel) {
    const char* context = _toContextString(loggingLevel);
    static const int concatCount = 6;
    char* effectiveFormat = concatenate(concatCount, context, "[", logger->name, "] ", format, "\n");
    if (ERROR <= loggingLevel) {
      _logInStream(stderr, effectiveFormat, arguments);
    } else {
      _logInStream(stdout, effectiveFormat, arguments);
    }
    free(effectiveFormat);
  }
}

/**
 * Gets the logging level from the specified string. Returns CRITICAL if the
 * provided value is unknown.
 */
static LoggingLevel _loggingLevelFromString(const char* loggingLevel) {
  if (strcmp(loggingLevel, "ALL") == 0) return ALL;
  else if (strcmp(loggingLevel, "DEBUGGING") == 0) return DEBUGGING;
  else if (strcmp(loggingLevel, "INFORMATION") == 0) return INFORMATION;
  else if (strcmp(loggingLevel, "WARNING") == 0) return WARNING;
  else if (strcmp(loggingLevel, "ERROR") == 0) return ERROR;
  else return CRITICAL;
}

/**
 * Low-level logging function.
 *
 * @see https://cplusplus.com/reference/cstdio/vfprintf/
 */
static void _logInStream(FILE* const stream, const char* const format, va_list arguments) {
  vfprintf(stream, format, arguments);
}

/**
 * Get the context string of the specified logging level.
 */
static const char* _toContextString(const LoggingLevel loggingLevel) {
  switch (loggingLevel) {
  case ALL:
    return "[ALL  ]";
  case DEBUGGING:
    return "[" DEBUGGING_COLOR "DEBUG" DEFAULT_COLOR "]";
  case INFORMATION:
    return "[" INFORMATION_COLOR "INFO " DEFAULT_COLOR "]";
  case WARNING:
    return "[" WARNING_COLOR "WARN " DEFAULT_COLOR "]";
  case ERROR:
    return "[" ERROR_COLOR "ERROR" DEFAULT_COLOR "]";
  default:
    return "[" CRITICAL_COLOR "FATAL" DEFAULT_COLOR "]";
  }
}

/* PUBLIC FUNCTIONS */

Logger* createLogger(char* name) {
  Logger* logger = calloc(1, sizeof(Logger));
  logger->loggingLevel = _loggingLevelFromString(getStringOrDefault("LOGGING_LEVEL", "INFORMATION"));
  const size_t length = 1 + strlen(name);
  logger->name = calloc(length, sizeof(char));
  strlcpy(logger->name, name, length);
  return logger;
}

void destroyLogger(Logger* logger) {
  if (logger != NULL) {
    if (logger->name != NULL) {
      free(logger->name);
    }
    free(logger);
  }
}

void logCritical(const Logger* logger, const char* const format, ...) {
  va_list arguments;
  va_start(arguments, format);
  _log(logger, CRITICAL, format, arguments);
  va_end(arguments);
}

void logDebugging(const Logger* logger, const char* const format, ...) {
  va_list arguments;
  va_start(arguments, format);
  _log(logger, DEBUGGING, format, arguments);
  va_end(arguments);
}

void logError(const Logger* logger, const char* const format, ...) {
  va_list arguments;
  va_start(arguments, format);
  _log(logger, ERROR, format, arguments);
  va_end(arguments);
}

void logInformation(const Logger* logger, const char* const format, ...) {
  va_list arguments;
  va_start(arguments, format);
  _log(logger, INFORMATION, format, arguments);
  va_end(arguments);
}

void logWarning(const Logger* logger, const char* const format, ...) {
  va_list arguments;
  va_start(arguments, format);
  _log(logger, WARNING, format, arguments);
  va_end(arguments);
}
