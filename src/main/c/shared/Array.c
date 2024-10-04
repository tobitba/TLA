#include "Array.h"
#include "ArrayTypes.h"
#include "Logger.h"
#include "String.h"
#include "utils.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define ARRAY_INSTANCE_NULL_EXIT exitInvalidArgument(__func__, "Array instance can't be NULL")

static Logger* _logger = NULL;

typedef struct ArrayCDT {
  size_t capacity;
  size_t length;
  ArrayElement* values;
  FreeEleFn freeEleFn;
  ToStringEleFn toStringEleFn;
} ArrayCDT;

int64_t toRealIdx(Array array, int64_t idx);
void growTo(Array array, size_t newCapacity);
void growBy(Array array, size_t extraCapacity);

/**
 * @param `initialCapacity` Initial capacity used for internal array.
 * @param `freeEleFn` Pointer to function that receives an `ArrayElement` and free's it.
 * @param `toStringEleFn` Pointer to function that receives an `ArrayElement` and returns a heap-allocated
 *         representation of the element.
 * @return A heap-allocated `Array`. The caller is responsible for freeing the allocated memory.
 */
Array Array_new(size_t initialCapacity, FreeEleFn freeEleFn, ToStringEleFn toStringEleFn) {
  ArrayCDT* array = malloc(sizeof(ArrayCDT));
  if (array == NULL) exitWithPerror(__func__, "malloc error");
  array->capacity = initialCapacity ? initialCapacity : 1;
  array->values = malloc(array->capacity * sizeof(ArrayElement));
  if (array->values == NULL) {
    free(array);
    exitWithPerror(__func__, "malloc error");
  }
  array->length = 0;
  array->freeEleFn = freeEleFn;
  array->toStringEleFn = toStringEleFn;
  return array;
}

void Array_free(Array array) {
  if (array == NULL) ARRAY_INSTANCE_NULL_EXIT;

  if (array->freeEleFn != NULL) {
    for (int32_t i = 0; i < array->length; ++i) array->freeEleFn(Array_get(array, i));
  }

  free(array->values);
  free(array);
}

ArrayElement Array_get(Array array, int64_t idx) {
  if (array == NULL) ARRAY_INSTANCE_NULL_EXIT;
  idx = toRealIdx(array, idx);
  if (idx < 0) exitInvalidArgument(__func__, "invalid index");
  return array->values[idx];
}

void Array_push(Array array, ArrayElement ele) {
  if (array == NULL) ARRAY_INSTANCE_NULL_EXIT;
  // else if (ele == NULL) exitInvalidArgument(__func__, "element to push can't be NULL");

  if (array->length >= array->capacity) growBy(array, array->capacity);

  array->values[array->length] = ele;
  ++array->length;
}

void Array_pop(Array array) {
  if (array == NULL) ARRAY_INSTANCE_NULL_EXIT;
  if (array->length == 0) return;
  if (array->freeEleFn != NULL) array->freeEleFn(Array_get(array, -1));
  --array->length;
}

size_t Array_getLen(Array array) {
  if (array == NULL) ARRAY_INSTANCE_NULL_EXIT;
  return array->length;
}

char* Array_toString(Array array) {
  if (array == NULL) ARRAY_INSTANCE_NULL_EXIT;
  if (array->toStringEleFn == NULL) exitInvalidArgument(__func__, "print element function not set");
  char* str = safeAsprintf("[ ");
  for (int i = 0; i < array->length; ++i) {
    char* eleStr = array->toStringEleFn(Array_get(array, i));
    char* newStr = safeAsprintf("%s%s%s", str, eleStr, i < array->length - 1 ? ", " : " ]");
    free(str);
    free(eleStr);
    str = newStr;
  }
  return str;
}

void Array_printInfo(Array array) {
  if (array == NULL) ARRAY_INSTANCE_NULL_EXIT;
  printf("{ \n");
  printf(
    "  capacity: %lu\n"
    "  length: %lu\n"
    "  array: %p\n"
    "  freeEleFn: %s\n"
    "  toStringEleFn: %s\n",
    array->capacity, array->length, (void*)array->values, array->freeEleFn ? "Assigned" : "NULL",
    array->toStringEleFn ? "Assigned" : "NULL"
  );
  printf("}\n");
}

//////////////////////////// Internal Functions ////////////////////////////

int64_t toRealIdx(Array array, int64_t idx) {
  // We need to first check if idx < 0 because if it's negative and we compare directly
  // against length (unsigned) then idx will be cast to unsigned and will be a huge number
  // almost certainly larger than length.
  if (idx < 0) {
    // Note: we negate idx because a->length is unsigned
    if (-idx > array->length) return -1;
    idx += (int64_t)array->length;
  } else if (idx >= array->length) return -1;
  return idx;
}

void growTo(Array array, size_t newCapacity) {
  void* aux = realloc(array->values, newCapacity * sizeof(ArrayElement));
  if (aux == NULL) exitWithPerror(__func__, "realloc error");
  array->capacity = newCapacity;
  array->values = aux;
}

void growBy(Array array, size_t extraCapacity) {
  growTo(array, array->capacity + extraCapacity);
}

void Array_initializeLogger() {
  _logger = createLogger("ArrayLib");
}

void Array_freeLogger() {
  if (_logger != NULL) {
    destroyLogger(_logger);
  }
}
