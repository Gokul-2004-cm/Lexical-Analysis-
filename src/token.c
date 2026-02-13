#include "token.h"

void initTokenList(TokenList *list) {
    list->count = 0;
}

void addToken(TokenList *list, TokenType type, const char *lexeme,
              int line, int col, int value) {
    if (list->count >= MAX_TOKENS) {
        fprintf(stderr, "Error: Token list overflow\n");
        return;
    }
    
    Token *token = &list->tokens[list->count];
    token->type = type;
    strncpy(token->lexeme, lexeme, MAX_LEXEME_LEN - 1);
    token->lexeme[MAX_LEXEME_LEN - 1] = '\0';
    token->lineNumber = line;
    token->columnNumber = col;
    token->tokenValue = value;
    
    list->count++;
}

const char* getTokenTypeString(TokenType type) {
    switch (type) {
        // Keywords
        case TOKEN_IF: return "IF";
        case TOKEN_ELSE: return "ELSE";
        case TOKEN_WHILE: return "WHILE";
        case TOKEN_FOR: return "FOR";
        case TOKEN_DO: return "DO";
        case TOKEN_RETURN: return "RETURN";
        case TOKEN_INT: return "INT";
        case TOKEN_FLOAT: return "FLOAT";
        case TOKEN_CHAR: return "CHAR";
        case TOKEN_VOID: return "VOID";
        case TOKEN_STRUCT: return "STRUCT";
        
        // Identifiers and Literals
        case TOKEN_ID: return "IDENTIFIER";
        case TOKEN_NUM: return "NUMBER";
        case TOKEN_CHAR_LIT: return "CHAR_LITERAL";
        case TOKEN_STRING: return "STRING";
        
        // Operators
        case TOKEN_PLUS: return "PLUS";
        case TOKEN_MINUS: return "MINUS";
        case TOKEN_MUL: return "MULTIPLY";
        case TOKEN_DIV: return "DIVIDE";
        case TOKEN_MOD: return "MODULO";
        case TOKEN_ASSIGN: return "ASSIGN";
        case TOKEN_EQ: return "EQUAL";
        case TOKEN_NE: return "NOT_EQUAL";
        case TOKEN_LT: return "LESS_THAN";
        case TOKEN_LE: return "LESS_EQUAL";
        case TOKEN_GT: return "GREATER_THAN";
        case TOKEN_GE: return "GREATER_EQUAL";
        case TOKEN_AND: return "LOGICAL_AND";
        case TOKEN_OR: return "LOGICAL_OR";
        case TOKEN_NOT: return "LOGICAL_NOT";
        case TOKEN_BITAND: return "BIT_AND";
        case TOKEN_BITOR: return "BIT_OR";
        case TOKEN_BITXOR: return "BIT_XOR";
        case TOKEN_LSHIFT: return "LEFT_SHIFT";
        case TOKEN_RSHIFT: return "RIGHT_SHIFT";
        case TOKEN_INC: return "INCREMENT";
        case TOKEN_DEC: return "DECREMENT";
        
        // Delimiters
        case TOKEN_LPAREN: return "LPAREN";
        case TOKEN_RPAREN: return "RPAREN";
        case TOKEN_LBRACE: return "LBRACE";
        case TOKEN_RBRACE: return "RBRACE";
        case TOKEN_LBRACKET: return "LBRACKET";
        case TOKEN_RBRACKET: return "RBRACKET";
        case TOKEN_SEMICOLON: return "SEMICOLON";
        case TOKEN_COMMA: return "COMMA";
        case TOKEN_DOT: return "DOT";
        case TOKEN_COLON: return "COLON";
        case TOKEN_ARROW: return "ARROW";
        
        case TOKEN_EOF: return "EOF";
        case TOKEN_ERROR: return "ERROR";
        case TOKEN_UNKNOWN: return "UNKNOWN";
        
        default: return "UNKNOWN_TOKEN_TYPE";
    }
}

void printTokens(TokenList *list, FILE *fp) {
    fprintf(fp, "=================================================\n");
    fprintf(fp, "%-5s | %-20s | %-15s | %-8s | %-8s\n",
            "No.", "Token Type", "Lexeme", "Line", "Column");
    fprintf(fp, "=================================================\n");
    
    for (int i = 0; i < list->count; i++) {
        Token *token = &list->tokens[i];
        fprintf(fp, "%-5d | %-20s | %-15s | %-8d | %-8d\n",
                i + 1,
                getTokenTypeString(token->type),
                token->lexeme,
                token->lineNumber,
                token->columnNumber);
    }
    
    fprintf(fp, "=================================================\n");
    fprintf(fp, "Total Tokens: %d\n", list->count);
}