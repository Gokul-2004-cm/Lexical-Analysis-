#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TOKENS 1000
#define MAX_LEXEME_LEN 100
#define MAX_TOKEN_LEN 50

typedef enum {
    // Keywords
    TOKEN_IF, TOKEN_ELSE, TOKEN_WHILE, TOKEN_FOR, TOKEN_DO, TOKEN_RETURN,
    TOKEN_INT, TOKEN_FLOAT, TOKEN_CHAR, TOKEN_VOID, TOKEN_STRUCT,
    
    // Identifiers and Literals
    TOKEN_ID, TOKEN_NUM, TOKEN_CHAR_LIT, TOKEN_STRING,
    
    // Operators
    TOKEN_PLUS, TOKEN_MINUS, TOKEN_MUL, TOKEN_DIV, TOKEN_MOD,
    TOKEN_ASSIGN, TOKEN_EQ, TOKEN_NE, TOKEN_LT, TOKEN_LE, TOKEN_GT, TOKEN_GE,
    TOKEN_AND, TOKEN_OR, TOKEN_NOT, TOKEN_BITAND, TOKEN_BITOR, TOKEN_BITXOR,
    TOKEN_LSHIFT, TOKEN_RSHIFT, TOKEN_INC, TOKEN_DEC,
    
    // Delimiters
    TOKEN_LPAREN, TOKEN_RPAREN, TOKEN_LBRACE, TOKEN_RBRACE,
    TOKEN_LBRACKET, TOKEN_RBRACKET, TOKEN_SEMICOLON, TOKEN_COMMA, TOKEN_DOT,
    TOKEN_COLON, TOKEN_ARROW,
    
    // Special
    TOKEN_EOF, TOKEN_ERROR, TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[MAX_LEXEME_LEN];
    int lineNumber;
    int columnNumber;
    int tokenValue;  // For numbers
} Token;

typedef struct {
    Token tokens[MAX_TOKENS];
    int count;
} TokenList;

// Function declarations
void initTokenList(TokenList *list);
void addToken(TokenList *list, TokenType type, const char *lexeme, 
              int line, int col, int value);
void printTokens(TokenList *list, FILE *fp);
const char* getTokenTypeString(TokenType type);

#endif