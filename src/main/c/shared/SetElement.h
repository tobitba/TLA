#ifndef SET_ELEMENT_H
#define SET_ELEMENT_H

#include "../frontend/syntactic-analysis/AbstractSyntaxTree.h"

typedef union SetElement {
  Symbol symbol;
  Production* production;
  ProductionRhsRule* productionRhsRule;
} SetElement;

#endif
