// token.h
#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_KEYWORD,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_SYMBOL,
} TokenType;

typedef struct {
    TokenType type;
    char* value;
} Token;

#endif // TOKEN_H