#include "Environment.h"
#include "Type.h"
#include <stdlib.h>
#include <string.h>

/* PUBLIC FUNCTIONS */

boolean getBooleanOrDefault(const char* name, const boolean defaultValue) {
  const char* value = getStringOrDefault(name, NULL);
  if (value == NULL) {
    return defaultValue;
  } else if (strcmp(value, "true") == 0) {
    return true;
  } else {
    return false;
  }
}

const char* getStringOrDefault(const char* name, const char* defaultValue) {
  const char* value = getenv(name);
  if (value == NULL) {
    return defaultValue;
  } else {
    return value;
  }
}
