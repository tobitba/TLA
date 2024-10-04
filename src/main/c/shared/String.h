#ifndef STRING_HEADER
#define STRING_HEADER

#include <stdarg.h>

enum ControlCharacters {
  CTRL_NUL = 0x00,
  CTRL_SOH = 0x01,
  CTRL_STX = 0x02,
  CTRL_ETX = 0x03,
  CTRL_EOT = 0x04,
  CTRL_ENQ = 0x05,
  CTRL_ACK = 0x06,
  CTRL_BEL = 0x07,
  CTRL_BS = 0x08,
  CTRL_HT = 0x09,
  CTRL_LF = 0x0A,
  CTRL_VT = 0x0B,
  CTRL_FF = 0x0C,
  CTRL_CR = 0x0D,
  CTRL_SO = 0x0E,
  CTRL_SI = 0x0F,
  CTRL_DLE = 0x10,
  CTRL_DC1 = 0x11,
  CTRL_DC2 = 0x12,
  CTRL_DC3 = 0x13,
  CTRL_DC4 = 0x14,
  CTRL_NAK = 0x15,
  CTRL_SYN = 0x16,
  CTRL_ETB = 0x17,
  CTRL_CAN = 0x18,
  CTRL_EM = 0x19,
  CTRL_SUB = 0x1A,
  CTRL_ESC = 0x1B,
  CTRL_FS = 0x1C,
  CTRL_GS = 0x1D,
  CTRL_RS = 0x1E,
  CTRL_US = 0x1F,
  CTRL_DEL = 0x7F
};

/**
 * Concatenates a list of strings. The returned string must be freed because
 * it uses heap-memory.
 *
 * @param count
 *	The size of the list.
 */
char* concatenate(const unsigned int count, ...);

/**
 * Creates a new string (using heap-memory), that is equal to the original,
 * but using escaped sequences for every control character (i.e., in the range
 * 0x00-0x1F and 0x7F).
 */
char* escape(const char* string);

/**
 * Generates an indentation string for the specified level, using heap-memory.
 *
 * @param character
 *	The character to use for spacing, for example ' ' or '\t'.
 * @param level
 *	The depth level to indent. Zero (0) means no-indentation.
 * @param size
 *	The size of every indentation level, thus, level N as a length of N*size
 *	characters.
 */
char* indentation(const char character, const unsigned int level, const unsigned int size);

char* safeAsprintf(const char* restrict fmt, ...);

#endif
