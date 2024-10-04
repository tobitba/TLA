#include "String.h"
#include "utils.h"
#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* PRIVATE FUNCTIONS */

static const char* _controlCharacterToEscapedString(const char character);

/**
 * Returns a read-only string that represents the escaped sequence of the
 * control character. If the character is not a control, then returns NULL.
 */
static const char* _controlCharacterToEscapedString(const char character) {
  switch (character) {
  case CTRL_NUL:
    return "\\0";
  case CTRL_SOH:
    return "\\x01";
  case CTRL_STX:
    return "\\x02";
  case CTRL_ETX:
    return "\\x03";
  case CTRL_EOT:
    return "\\x04";
  case CTRL_ENQ:
    return "\\x05";
  case CTRL_ACK:
    return "\\x06";
  case CTRL_BEL:
    return "\\a";
  case CTRL_BS:
    return "\\b";
  case CTRL_HT:
    return "\\t";
  case CTRL_LF:
    return "\\n";
  case CTRL_VT:
    return "\\v";
  case CTRL_FF:
    return "\\f";
  case CTRL_CR:
    return "\\r";
  case CTRL_SO:
    return "\\x0E";
  case CTRL_SI:
    return "\\x0F";
  case CTRL_DLE:
    return "\\x10";
  case CTRL_DC1:
    return "\\x11";
  case CTRL_DC2:
    return "\\x12";
  case CTRL_DC3:
    return "\\x13";
  case CTRL_DC4:
    return "\\x14";
  case CTRL_NAK:
    return "\\x15";
  case CTRL_SYN:
    return "\\x16";
  case CTRL_ETB:
    return "\\x17";
  case CTRL_CAN:
    return "\\x18";
  case CTRL_EM:
    return "\\x19";
  case CTRL_SUB:
    return "\\x1A";
  case CTRL_ESC:
    return "\\x1B";
  case CTRL_FS:
    return "\\x1C";
  case CTRL_GS:
    return "\\x1D";
  case CTRL_RS:
    return "\\x1E";
  case CTRL_US:
    return "\\x1F";
  case CTRL_DEL:
    return "\\x7F";
  default:
    return NULL;
  }
}

/* PUBLIC FUNCTIONS */

char* concatenate(const unsigned int count, ...) {
  va_list arguments;
  va_start(arguments, count);
  unsigned int length = 1;
  for (unsigned int k = 0; k < count; ++k) {
    const char* nextString = va_arg(arguments, const char*);
    length += strlen(nextString);
  }
  va_end(arguments);
  char* string = calloc(length, sizeof(char));
  va_start(arguments, count);
  for (unsigned int k = 0; k < count; ++k) {
    const char* nextString = va_arg(arguments, const char*);
    strlcat(string, nextString, length);
  }
  va_end(arguments);
  return string;
}

char* escape(const char* string) {
  unsigned int length = 1;
  for (unsigned int k = 0; 0 < string[k]; ++k) {
    if (iscntrl(string[k])) {
      length += strlen(_controlCharacterToEscapedString(string[k]));
    } else {
      length += 1;
    }
  }
  char* escapedString = calloc(length, sizeof(char));
  char charToString[2] = {0, 0};
  for (unsigned int k = 0; 0 < string[k]; ++k) {
    if (iscntrl(string[k])) {
      strlcat(escapedString, _controlCharacterToEscapedString(string[k]), length);
    } else {
      charToString[0] = string[k];
      strlcat(escapedString, charToString, length);
    }
  }
  return escapedString;
}

char* indentation(const char character, const unsigned int level, const unsigned int size) {
  const unsigned int indentationLength = level * size;
  char* indentation = calloc(1 + indentationLength, sizeof(char));
  for (int k = 0; k < indentationLength; ++k) {
    indentation[k] = character;
  }
  return indentation;
}

char* safeAsprintf(const char* restrict fmt, ...) {
  va_list arguments;
  va_start(arguments, fmt);
  char* str;
  int err = vasprintf(&str, fmt, arguments);
  va_end(arguments);
  if (err < 0) exitWithErrno(ENOMEM, __func__, "@asprintf error");
  return str;
}
