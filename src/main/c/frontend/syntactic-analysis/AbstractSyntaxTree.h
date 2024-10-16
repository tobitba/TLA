#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

/** Initialize module's internal state. */
void initializeAbstractSyntaxTreeModule();

/** Shutdown module's internal state. */
void shutdownAbstractSyntaxTreeModule();

// Forward declaration of Array to avoid circular references.
typedef struct ArrayCDT* Array;
typedef struct SetCDT* Set;

/**
 * Node types for the Abstract Syntax Tree (AST).
 */

typedef enum { GRAMMAR_DEFINITION, SYMBOL_SET, PRODUCTION_SET, LANGUAGE_SENTENCE } SentenceType;

typedef enum { SYMBOL_SYMBOL_T, SYMBOL_T, LAMBDA_T } ProductionRhsRuleType;

typedef enum {
  LANGUAGE,
  LANG_UNION,
  LANG_INTERSEC,
  LANG_MINUS,
  LANG_CONCAT,
  LANG_REVERSE,
  LANG_COMPLEMENT
} LanguageExpressionType;

typedef enum { GRAMMAR_ID, LANGUAGE_ID } LanguageIdType;

/**
 * This typedefs allows self-referencing types.
 */

typedef struct Id Id;
typedef struct Symbol Symbol;

typedef struct Program Program;
typedef struct Sentence Sentence;
typedef struct GrammarDefinition GrammarDefinition;
typedef struct SymbolSetBinding SymbolSetBinding;
typedef struct ProductionSetBinding ProductionSetBinding;
typedef struct Production Production;
typedef struct ProductionRhsRule ProductionRhsRule;
typedef struct LanguageBinding LanguageBinding;
typedef struct LanguageExpression LanguageExpression;
typedef struct Language Language;

typedef Array SentenceArray;
typedef Set SymbolSet;
typedef Set ProductionSet;
typedef Set ProductionRhsRuleSet;

struct Id {
  char* id;
  int length;
};

struct Symbol {
  char* symbol;
  int length;
};

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

struct SymbolSetBinding {
  Id id;
  SymbolSet symbols;
};

struct LanguageBinding {
  Id id;
  LanguageExpression* LanguageExpression;
};

struct LanguageExpression {
  union {
    Language* language;
    LanguageExpression* unaryLanguageExpression;
    struct {
      LanguageExpression* leftLanguageExpression;
      LanguageExpression* rightLanguageExpression;
    };
  };
  LanguageExpressionType type;
};

struct Language {
  LanguageIdType type;
  Id id;
};

struct ProductionSetBinding {
  Id id;
  ProductionSet productions;
};

struct Sentence {
  union {
    GrammarDefinition* grammarDefinition;
    SymbolSetBinding* symbolSetBinding;
    ProductionSetBinding* productionSetBinding;
    LanguageBinding* languageBinding;
  };
  SentenceType type;
};

struct Production {
  Symbol lhs;
  ProductionRhsRuleSet rhs;
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
