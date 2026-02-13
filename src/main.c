#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>

void readInputFile(const char *filename, char *buffer, int maxLen) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Cannot open file '%s'\n", filename);
        exit(1);
    }
    
    int totalRead = 0;
    int bytesRead;
    while ((bytesRead = fread(buffer + totalRead, 1, maxLen - totalRead - 1, file)) > 0) {
        totalRead += bytesRead;
    }
    buffer[totalRead] = '\0';
    
    fclose(file);
}

void writeTokensToFile(TokenList *tokenList, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error: Cannot create output file '%s'\n", filename);
        return;
    }
    
    printTokens(tokenList, file);
    fclose(file);
    printf("Tokens written to: %s\n", filename);
}

void writeSymbolTableToFile(SymbolTable *symbolTable, const char *filename) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        fprintf(stderr, "Error: Cannot open output file '%s'\n", filename);
        return;
    }
    
    printSymbolTable(symbolTable, file);
    fclose(file);
    printf("Symbol table written to: %s\n", filename);
}

int main() {
    printf("\n╔════════════════════════════════════════════╗\n");
    printf("║   LEXICAL ANALYZER - Compiler Design       ║\n");
    printf("║   Author: Gokul-2004-cm                    ║\n");
    printf("╚════════════════════════════════════════════╝\n\n");
    
    Lexer lexer;
    char input[MAX_INPUT_LEN];
    
    // Read from input file
    printf("Reading input from: input/input.txt\n");
    readInputFile("input/input.txt", input, MAX_INPUT_LEN);
    
    // Initialize and tokenize
    initLexer(&lexer, input);
    printf("Tokenizing...\n\n");
    tokenize(&lexer);
    
    // Display analysis report
    analyzeLexer(&lexer);
    
    // Print tokens to console and file
    printf("\n========== TOKEN LIST ==========\n");
    printTokens(&lexer.tokenList, stdout);
    writeTokensToFile(&lexer.tokenList, "output/tokens.txt");
    
    // Print symbol table to console and file
    printf("\n========== SYMBOL TABLE ==========\n");
    printSymbolTable(&lexer.symbolTable, stdout);
    writeSymbolTableToFile(&lexer.symbolTable, "output/tokens.txt");
    
    // Error and warning summary
    if (lexer.errorCount == 0) {
        printf("\n✓ No errors found!\n");
    } else {
        printf("\n✗ %d error(s) found!\n", lexer.errorCount);
    }
    
    if (lexer.warningCount > 0) {
        printf("⚠ %d warning(s) found!\n", lexer.warningCount);
    }
    
    return 0;
}