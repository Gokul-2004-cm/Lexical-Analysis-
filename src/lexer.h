#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include "symbolTable.h"

#define MAX_INPUT_LEN 10000

typedef struct {
    char input[MAX_INPUT_LEN];
    int position;
    int lineNumber;
    int columnNumber;
    TokenList tokenList;
    SymbolTable symbolTable;
    int errorCount;
    int warningCount;
} Lexer;

// Function declarations
void initLexer(Lexer *lexer, const char *input);
void tokenize(Lexer *lexer);
char getCurrentChar(Lexer *lexer);
char peekChar(Lexer *lexer, int offset);
void advance(Lexer *lexer);
int isKeyword(const char *lexeme);
TokenType getKeywordType(const char *lexeme);
int isOperator(char c);
void scanNumber(Lexer *lexer, char *buffer);
void scanIdentifier(Lexer *lexer, char *buffer);
void scanString(Lexer *lexer, char *buffer);
void scanCharLiteral(Lexer *lexer, char *buffer);
void skipWhitespace(Lexer *lexer);
void skipComment(Lexer *lexer);
void reportError(Lexer *lexer, const char *message);
void reportWarning(Lexer *lexer, const char *message);
void analyzeLexer(Lexer *lexer);

#endif