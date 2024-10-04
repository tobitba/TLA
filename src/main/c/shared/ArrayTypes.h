#ifndef ARRAY_TYPES_H
#define ARRAY_TYPES_H

#include "../frontend/syntactic-analysis/AbstractSyntaxTree.h"

typedef union ArrayElement {
  Sentence* sentence;
  Symbol symbol;
  Production* production;
  StringElement stringElement;
  String string;
} ArrayElement;

#endif
