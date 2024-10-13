#include "Set.h"
#include "Logger.h"
#include "SetElement.h"
#include "String.h"
#include "utils.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SET_INSTANCE_NULL exitInvalidArgument(__func__, "Set instance can't be NULL")
#define SET_ITER_INSTANCE_NULL exitInvalidArgument(__func__, "SetIterator instance can't be NULL")

static Logger* _logger = NULL;

typedef struct Node {
  uint32_t hash;
  SetElement element;
  struct Node* next;
} Node;

typedef struct SetCDT {
  size_t capacity;
  size_t count;
  Node** nodes;
  Set_HashEleFn hashEleFn;
  Set_EqualsEleFn equalsEleFn;
  Set_FreeEleFn freeEleFn;
  Set_ToStringEleFn toStringEleFn;
} SetCDT;

typedef struct SetIteratorCDT {
  uint32_t idx;
  Node** nodes;
  Node* node;
  size_t capacity;
} SetIteratorCDT;

enum { INITIAL_CAPACITY = 100 };

static uint32_t hashIdx(Set set, SetElement ele, uint32_t* idx);
static void growTo(Set set, size_t newCapacity);
static void growBy(Set set, size_t extraCapacity);
static Node* Node_new(SetElement ele, uint32_t hash);

Set Set_new(
  Set_HashEleFn hashEleFn, Set_EqualsEleFn equalsEleFn, Set_FreeEleFn freeEleFn, Set_ToStringEleFn toStringEleFn
) {
  if (hashEleFn == NULL || equalsEleFn == NULL) {
    exitInvalidArgument(__func__, "Both `hashEleFn` and `equalsEleFn` are required arguments");
  }
  SetCDT* set = malloc(sizeof(SetCDT));
  if (set == NULL) exitWithPerror(__func__, "malloc error");
  set->capacity = INITIAL_CAPACITY;
  set->nodes = (Node**)calloc(set->capacity, sizeof(Node*));
  if (set->nodes == NULL) {
    free(set);
    exitWithPerror(__func__, "malloc error");
  }
  set->count = 0;
  set->hashEleFn = hashEleFn;
  set->equalsEleFn = equalsEleFn;
  set->freeEleFn = freeEleFn;
  set->toStringEleFn = toStringEleFn;
  return set;
}

void Set_free(Set set) {
  if (set == NULL) SET_INSTANCE_NULL;
  // size_t count = 0;
  for (int i = 0; i < set->capacity; ++i) {
    Node* node = set->nodes[i];
    while (node != NULL) {
      if (set->freeEleFn != NULL) set->freeEleFn(node->element);
      Node* prev = node;
      node = node->next;
      free(prev);
      // if (++count >= set->length) return;
    }
  }
  free((void*)set->nodes);
  free(set);
}

void Set_freeNotElements(Set set) {
  if (set == NULL) SET_INSTANCE_NULL;
  set->freeEleFn = NULL;
  Set_free(set);
}

bool Set_add(Set set, SetElement ele) {
  if (set == NULL) SET_INSTANCE_NULL;
  uint32_t idx;
  uint32_t hash = hashIdx(set, ele, &idx);
  Node* node = set->nodes[idx];
  if (node == NULL) {
    set->nodes[idx] = Node_new(ele, hash);
  } else {
    Node* prev;
    while (node != NULL) {
      if (node->hash == hash) {
        if (set->equalsEleFn(node->element, ele)) {
          if (set->freeEleFn != NULL) set->freeEleFn(ele);
          return false;
        }
      }
      prev = node;
      node = node->next;
    }
    prev->next = Node_new(ele, hash);
  }
  ++set->count;
  return true;
}

Node* recDelete(Node* node, SetElement ele, bool* flag, Set set) {
  if (node == NULL) return NULL;
  if (set->equalsEleFn(node->element, ele)) {
    *flag = true;
    Node* aux = node->next;
    char* eleStr = set->toStringEleFn(node->element);
    logDebugging(_logger, "Deleting element: %s", eleStr);
    free(eleStr);
    if (set->freeEleFn != NULL) {
      set->freeEleFn(node->element);
    }
    // free(node);
    return aux;
  }
  node->next = recDelete(node->next, ele, flag, set);
  return node;
}

bool Set_remove(Set set, SetElement ele) {
  if (set == NULL) SET_INSTANCE_NULL;
  bool removalFlag = false;
  uint32_t idx;
  hashIdx(set, ele, &idx);
  logDebugging(_logger, "Attempting to remove element from set.");
  char* setStr = Set_toString(set);
  char* eleStr = set->toStringEleFn(ele);
  logDebugging(_logger, "Set: %s, Element: %s", setStr, eleStr);
  free(setStr);
  free(eleStr);
  if (set->nodes[idx] != NULL) {
    set->nodes[idx] = recDelete(set->nodes[idx], ele, &removalFlag, set);
  }
  if (removalFlag) {
    set->count--;
  }
  Set_printInfo(set);
  return removalFlag;
}

bool Set_isEmpty(Set set) {
  return set->count == 0;
}

SetElement* Set_find(Set set, SetElement ele) {
  if (set == NULL) SET_INSTANCE_NULL;
  uint32_t idx;
  uint32_t hash = hashIdx(set, ele, &idx);
  Node* node = set->nodes[idx];
  while (node != NULL) {
    if (node->hash == hash) {
      if (set->equalsEleFn(node->element, ele)) return &node->element;
    }
    node = node->next;
  }
  return NULL;
}

bool Set_Has(Set set, SetElement ele) {
  return Set_find(set, ele) != NULL;
}

void Set_union(Set dest, Set src) {
  if (dest == NULL) SET_INSTANCE_NULL;
  if (src == NULL) return;

  for (int i = 0; i < src->capacity; ++i) {
    Node* node = src->nodes[i];
    Node* prev;
    while (node != NULL) {
      Set_add(dest, node->element);
      prev = node;
      node = node->next;
      // `src` nodes should always be freed as they will be recreated in `dest` by `Set_add`.
      free(prev);
    }
  }
  free((void*)src->nodes);
  free(src);
}

void Set_intersection(Set base, Set filter) {
  if (base == NULL) SET_INSTANCE_NULL;
  if (filter == NULL) {
    base = NULL;
    return;
  }
  for (int i = 0; i < base->capacity; ++i) {
    Node* node = base->nodes[i];
    while (node != NULL) {
      if (!Set_Has(filter, node->element)) Set_remove(base, node->element);
      node = node->next;
    }
  }
  free((void*)filter->nodes);
  free(filter);
}

void Set_subtraction(Set minuend, Set subtrahend) {
  if (minuend == NULL) SET_INSTANCE_NULL;
  if (subtrahend == NULL) return;
  char* min = Set_toString(minuend);
  char* sub = Set_toString(subtrahend);
  logError(_logger, "min: %s, sub: %s", min, sub);
  for (int i = 0; i < subtrahend->capacity; ++i) {
    Node* node = subtrahend->nodes[i];
    while (node != NULL) {
      Set_remove(minuend, node->element);
      node = node->next;
    }
  }
}

char* Set_toString(Set set) {
  if (set == NULL) SET_INSTANCE_NULL;
  if (set->toStringEleFn == NULL) exitInvalidArgument(__func__, "print element function not set");
  if (set->count == 0) return safeAsprintf("{}");
  char* str = safeAsprintf("{ ");

  for (int i = 0; i < set->capacity; ++i) {
    Node* node = set->nodes[i];
    if (node == NULL) continue;
    while (node != NULL) {
      char* eleStr = set->toStringEleFn(node->element);
      char* newStr = safeAsprintf("%s%s, ", str, eleStr);
      free(str);
      free(eleStr);
      str = newStr;
      node = node->next;
    }
  }

  size_t len = strlen(str);
  str[len - 2] = ' ';
  str[len - 1] = '}';

  return str;
}

void Set_printInfo(Set set) {
  if (set == NULL) SET_INSTANCE_NULL;
  printf("{ \n");
  printf(
    "  capacity:       %lu\n"
    "  count:          %lu\n"
    "  nodes:          %p\n"
    "  hashEleFn:      %s\n"
    "  equalsEleFn:    %s\n"
    "  freeEleFn:      %s\n"
    "  toStringEleFn:  %s\n",
    set->capacity, set->count, (void*)set->nodes, set->hashEleFn ? "Assigned" : "NULL",
    set->equalsEleFn ? "Assigned" : "NULL", set->freeEleFn ? "Assigned" : "NULL",
    set->toStringEleFn ? "Assigned" : "NULL"
  );
  printf("}\n");
}

SetIterator SetIterator_new(Set set) {
  if (set == NULL) SET_INSTANCE_NULL;
  SetIterator iterator = safeMalloc(sizeof(SetIteratorCDT));
  for (int i = 0; i < set->capacity; ++i) {
    Node* node = set->nodes[i];
    if (node != NULL) {
      iterator->idx = i;
      iterator->nodes = set->nodes;
      iterator->node = node;
      iterator->capacity = set->capacity;
      return iterator;
    }
  }
  iterator->node = NULL;
  return iterator;
}

void SetIterator_free(SetIterator iter) {
  free(iter);
}

bool SetIterator_hasNext(SetIterator iter) {
  if (iter == NULL) SET_ITER_INSTANCE_NULL;
  return iter->node != NULL;
}

SetElement* SetIterator_next(SetIterator iter) {
  if (iter == NULL) SET_ITER_INSTANCE_NULL;
  if (iter->node == NULL) return NULL;
  SetElement* ele = &iter->node->element;
  iter->node = iter->node->next;
  if (iter->node == NULL) {
    for (++iter->idx; iter->idx < iter->capacity; ++iter->idx) {
      iter->node = iter->nodes[iter->idx];
      if (iter->node != NULL) break;
    }
  }

  return ele;
}

//////////////////////////// Internal Functions ////////////////////////////

uint32_t hashIdx(Set set, SetElement ele, uint32_t* idx) {
  uint32_t hash = set->hashEleFn(ele);
  *idx = hash % set->capacity;
  return hash;
}

void growTo(Set set, size_t newCapacity) {
  Node** aux = (Node**)realloc((void*)set->nodes, newCapacity * sizeof(Node*));
  if (aux == NULL) exitWithPerror(__func__, "realloc error");
  set->capacity = newCapacity;
  set->nodes = aux;
}

void growBy(Set set, size_t extraCapacity) {
  growTo(set, set->capacity + extraCapacity);
}

Node* Node_new(SetElement ele, uint32_t hash) {
  Node* node = safeMalloc(sizeof(Node));
  node->next = NULL;
  node->element = ele;
  node->hash = hash;
  return node;
}

void Set_initializeLogger() {
  _logger = createLogger("SetLib");
}

void Set_freeLogger() {
  if (_logger != NULL) {
    destroyLogger(_logger);
  }
}
