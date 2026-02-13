#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SYMBOLS 500
#define MAX_SYMBOL_LEN 100

typedef enum {
    SYMBOL_VARIABLE, SYMBOL_FUNCTION, SYMBOL_KEYWORD, SYMBOL_ARRAY, SYMBOL_STRUCT
} SymbolType;

typedef struct {
    char name[MAX_SYMBOL_LEN];
    SymbolType type;
    char dataType[MAX_SYMBOL_LEN];
    int scope;
    int lineNumber;
    int usage;  // Number of times used
} Symbol;

typedef struct {
    Symbol symbols[MAX_SYMBOLS];
    int count;
} SymbolTable;

// Function declarations
void initSymbolTable(SymbolTable *table);
int addSymbol(SymbolTable *table, const char *name, SymbolType type,
              const char *dataType, int scope, int lineNumber);
Symbol* lookupSymbol(SymbolTable *table, const char *name);
void updateSymbolUsage(SymbolTable *table, const char *name);
void printSymbolTable(SymbolTable *table, FILE *fp);
int isDuplicate(SymbolTable *table, const char *name, int scope);

#endif