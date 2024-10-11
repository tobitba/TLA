#ifndef SET_H
#define SET_H

// AbstractSyntaxTree.h has to be included because clang is stupid when dealing with forward declarations...
#include "../../c/frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "SetElement.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#pragma clang diagnostic ignored "-Wtypedef-redefinition"
typedef struct SetCDT* Set;

typedef struct SetIteratorCDT* SetIterator;

typedef uint32_t (*Set_HashEleFn)(SetElement ele);
typedef bool (*Set_EqualsEleFn)(SetElement ele1, SetElement ele2);
typedef void (*Set_FreeEleFn)(SetElement ele);

/**
 * @typedef `ToStringEleFn`
 *
 * @param `ele` The element to be converted to a string.
 * @return A heap-allocated `char*` representing the element.
 *         The caller is responsible for freeing the allocated memory.
 */
typedef char* (*Set_ToStringEleFn)(SetElement ele);

Set Set_new(
  Set_HashEleFn hashEleFn, Set_EqualsEleFn equalsEleFn, Set_FreeEleFn freeEleFn, Set_ToStringEleFn toStringEleFn
);
void Set_free(Set set);

/**
 * If `ele` is already in `set` and `freeEleFn` was set on initialization,
 * then `ele` will be freed by this function.
 *
 * @return `true` if element was inserted, `false` if it was already present.
 */
bool Set_add(Set set, SetElement ele);

/**
 * @param `dest` Destination array, all elements from `src` will be pushed to `dest`.
 * @param `src` Source array. Will be freed after call to prevent double free errors on the elements.
 */
void Set_union(Set dest, Set src);

SetElement* Set_find(Set set, SetElement ele);
bool Set_Has(Set set, SetElement ele);
char* Set_toString(Set set);
void Set_printInfo(Set set);

SetIterator Set_iterator(Set set);
bool SetIterator_hasNext(SetIterator iter);
SetElement* SetIterator_next(SetIterator iter);

#endif
