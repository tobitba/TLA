#ifndef ARRAY_TYPES_H
#define ARRAY_TYPES_H

#include "../frontend/syntactic-analysis/AbstractSyntaxTree.h"

typedef union ArrayElement {
  Sentence* sentence;
  Symbol symbol;
  Production* production;
  ProductionRhsRule* productionRhsRule;
} ArrayElement;

#endif
