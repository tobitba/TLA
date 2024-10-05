#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

/** Initialize module's internal state. */
void initializeAbstractSyntaxTreeModule();

/** Shutdown module's internal state. */
void shutdownAbstractSyntaxTreeModule();

// Forward declaration of Array to avoid circular references.
typedef struct ArrayCDT* Array;

/**
 * Node types for the Abstract Syntax Tree (AST).
 */

typedef enum { GRAMMAR_DEFINITION, SYMBOL_SET, PRODUCTION_SET } SentenceType;

typedef enum { SYMBOL_SYMBOL_T, SYMBOL_T, LAMBDA_T } ProductionRhsRuleType;

/**
 * This typedefs allows self-referencing types.
 */

typedef char* Id;
typedef char* Symbol;

typedef struct Program Program;
typedef struct Sentence Sentence;
typedef struct GrammarDefinition GrammarDefinition;
typedef struct SymbolSet SymbolSet;
typedef struct ProductionSet ProductionSet;
typedef struct Production Production;
typedef struct ProductionRhsRule ProductionRhsRule;

typedef Array SentenceArray;
typedef Array SymbolArray;
typedef Array ProductionArray;
typedef Array ProductionRhsRuleArray;

struct Program {
  SentenceArray sentences;
};

struct GrammarDefinition {
  Id id;
  Id terminalSetId;
  Id nonTerminalSetId;
  Id productionSetId;
  Id initialSymbolId;
};

struct SymbolSet {
  Id id;
  SymbolArray symbols;
};

struct ProductionSet {
  Id id;
  ProductionArray productions;
};

struct Sentence {
  union {
    GrammarDefinition* grammarDefinition;
    SymbolSet* symbolSet;
    ProductionSet* productionSet;
  };
  SentenceType type;
};

struct Production {
  Symbol lhs;
  ProductionRhsRuleArray rhs;
};

struct ProductionRhsRule {
  union {
    struct {
      Symbol leftSymbol;
      Symbol rightSymbol;
    };
    Symbol symbol;
  };
  ProductionRhsRuleType type;
};

/**
 * Node recursive destructors.
 */
// void releaseConstant(Constant* constant);
// void releaseExpression(Expression* expression);
// void releaseFactor(Factor* factor);
void releaseProgram(Program* program);

#endif
