#include "symbolTable.h"

void initSymbolTable(SymbolTable *table) {
    table->count = 0;
}

int isDuplicate(SymbolTable *table, const char *name, int scope) {
    for (int i = 0; i < table->count; i++) {
        if (strcmp(table->symbols[i].name, name) == 0 && 
            table->symbols[i].scope == scope) {
            return 1;
        }
    }
    return 0;
}

int addSymbol(SymbolTable *table, const char *name, SymbolType type,
              const char *dataType, int scope, int lineNumber) {
    if (table->count >= MAX_SYMBOLS) {
        fprintf(stderr, "Error: Symbol table overflow\n");
        return 0;
    }
    
    if (isDuplicate(table, name, scope)) {
        fprintf(stderr, "Warning: Duplicate symbol '%s' at line %d\n", name, lineNumber);
        return 0;
    }
    
    Symbol *sym = &table->symbols[table->count];
    strncpy(sym->name, name, MAX_SYMBOL_LEN - 1);
    sym->name[MAX_SYMBOL_LEN - 1] = '\0';
    sym->type = type;
    strncpy(sym->dataType, dataType, MAX_SYMBOL_LEN - 1);
    sym->dataType[MAX_SYMBOL_LEN - 1] = '\0';
    sym->scope = scope;
    sym->lineNumber = lineNumber;
    sym->usage = 0;
    
    table->count++;
    return 1;
}

Symbol* lookupSymbol(SymbolTable *table, const char *name) {
    for (int i = 0; i < table->count; i++) {
        if (strcmp(table->symbols[i].name, name) == 0) {
            return &table->symbols[i];
        }
    }
    return NULL;
}

void updateSymbolUsage(SymbolTable *table, const char *name) {
    Symbol *sym = lookupSymbol(table, name);
    if (sym != NULL) {
        sym->usage++;
    }
}

const char* getSymbolTypeString(SymbolType type) {
    switch (type) {
        case SYMBOL_VARIABLE: return "Variable";
        case SYMBOL_FUNCTION: return "Function";
        case SYMBOL_KEYWORD: return "Keyword";
        case SYMBOL_ARRAY: return "Array";
        case SYMBOL_STRUCT: return "Struct";
        default: return "Unknown";
    }
}

void printSymbolTable(SymbolTable *table, FILE *fp) {
    fprintf(fp, "\n=================================================================\n");
    fprintf(fp, "%-20s | %-15s | %-12s | %-6s | %-6s | %-6s\n",
            "Symbol Name", "Symbol Type", "Data Type", "Scope", "Line", "Usage");
    fprintf(fp, "=================================================================\n");
    
    for (int i = 0; i < table->count; i++) {
        Symbol *sym = &table->symbols[i];
        fprintf(fp, "%-20s | %-15s | %-12s | %-6d | %-6d | %-6d\n",
                sym->name,
                getSymbolTypeString(sym->type),
                sym->dataType,
                sym->scope,
                sym->lineNumber,
                sym->usage);
    }
    
    fprintf(fp, "=================================================================\n");
    fprintf(fp, "Total Symbols: %d\n\n", table->count);
}