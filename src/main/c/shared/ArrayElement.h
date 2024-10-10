#ifndef ARRAY_ELEMENT_H
#define ARRAY_ELEMENT_H

#include "../frontend/syntactic-analysis/AbstractSyntaxTree.h"

typedef union ArrayElement {
  Sentence* sentence;
  Symbol symbol;
  Production* production;
  ProductionRhsRule* productionRhsRule;
} ArrayElement;

#endif
