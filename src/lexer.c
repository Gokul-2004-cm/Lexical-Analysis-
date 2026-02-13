#include "lexer.h"
#include <ctype.h>

const char *keywords[] = {
    "if", "else", "while", "for", "do", "return",
    "int", "float", "char", "void", "struct"
};

TokenType keywordTypes[] = {
    TOKEN_IF, TOKEN_ELSE, TOKEN_WHILE, TOKEN_FOR, TOKEN_DO, TOKEN_RETURN,
    TOKEN_INT, TOKEN_FLOAT, TOKEN_CHAR, TOKEN_VOID, TOKEN_STRUCT
};

#define NUM_KEYWORDS 11

void initLexer(Lexer *lexer, const char *input) {
    strncpy(lexer->input, input, MAX_INPUT_LEN - 1);
    lexer->input[MAX_INPUT_LEN - 1] = '\0';
    lexer->position = 0;
    lexer->lineNumber = 1;
    lexer->columnNumber = 1;
    lexer->errorCount = 0;
    lexer->warningCount = 0;
    initTokenList(&lexer->tokenList);
    initSymbolTable(&lexer->symbolTable);
}

char getCurrentChar(Lexer *lexer) {
    if (lexer->position >= strlen(lexer->input)) {
        return '\0';
    }
    return lexer->input[lexer->position];
}

char peekChar(Lexer *lexer, int offset) {
    int pos = lexer->position + offset;
    if (pos >= strlen(lexer->input)) {
        return '\0';
    }
    return lexer->input[pos];
}

void advance(Lexer *lexer) {
    if (lexer->input[lexer->position] == '\n') {
        lexer->lineNumber++;
        lexer->columnNumber = 1;
    } else {
        lexer->columnNumber++;
    }
    lexer->position++;
}

int isKeyword(const char *lexeme) {
    for (int i = 0; i < NUM_KEYWORDS; i++) {
        if (strcmp(lexeme, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

TokenType getKeywordType(const char *lexeme) {
    for (int i = 0; i < NUM_KEYWORDS; i++) {
        if (strcmp(lexeme, keywords[i]) == 0) {
            return keywordTypes[i];
        }
    }
    return TOKEN_UNKNOWN;
}

int isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' ||
            c == '=' || c == '!' || c == '<' || c == '>' || c == '&' ||
            c == '|' || c == '^' || c == '~');
}

void skipWhitespace(Lexer *lexer) {
    while (getCurrentChar(lexer) == ' ' || getCurrentChar(lexer) == '\t' ||
           getCurrentChar(lexer) == '\n' || getCurrentChar(lexer) == '\r') {
        advance(lexer);
    }
}

void skipComment(Lexer *lexer) {
    if (getCurrentChar(lexer) == '/' && peekChar(lexer, 1) == '/') {
        while (getCurrentChar(lexer) != '\n' && getCurrentChar(lexer) != '\0') {
            advance(lexer);
        }
    } else if (getCurrentChar(lexer) == '/' && peekChar(lexer, 1) == '*') {
        advance(lexer);
        advance(lexer);
        while (!(getCurrentChar(lexer) == '*' && peekChar(lexer, 1) == '/')) {
            if (getCurrentChar(lexer) == '\0') {
                reportError(lexer, "Unterminated comment");
                break;
            }
            advance(lexer);
        }
        if (getCurrentChar(lexer) != '\0') {
            advance(lexer);
            advance(lexer);
        }
    }
}

void scanNumber(Lexer *lexer, char *buffer) {
    int bufIndex = 0;
    while (isdigit(getCurrentChar(lexer)) || getCurrentChar(lexer) == '.') {
        buffer[bufIndex++] = getCurrentChar(lexer);
        advance(lexer);
    }
    buffer[bufIndex] = '\0';
}

void scanIdentifier(Lexer *lexer, char *buffer) {
    int bufIndex = 0;
    while (isalnum(getCurrentChar(lexer)) || getCurrentChar(lexer) == '_') {
        buffer[bufIndex++] = getCurrentChar(lexer);
        advance(lexer);
    }
    buffer[bufIndex] = '\0';
}

void scanString(Lexer *lexer, char *buffer) {
    int bufIndex = 0;
    buffer[bufIndex++] = '"';
    advance(lexer);
    
    while (getCurrentChar(lexer) != '"' && getCurrentChar(lexer) != '\0') {
        if (getCurrentChar(lexer) == '\\') {
            buffer[bufIndex++] = getCurrentChar(lexer);
            advance(lexer);
            if (getCurrentChar(lexer) != '\0') {
                buffer[bufIndex++] = getCurrentChar(lexer);
                advance(lexer);
            }
        } else {
            buffer[bufIndex++] = getCurrentChar(lexer);
            advance(lexer);
        }
    }
    
    if (getCurrentChar(lexer) == '"') {
        buffer[bufIndex++] = '"';
        advance(lexer);
    } else {
        reportError(lexer, "Unterminated string");
    }
    buffer[bufIndex] = '\0';
}

void scanCharLiteral(Lexer *lexer, char *buffer) {
    int bufIndex = 0;
    buffer[bufIndex++] = '\'';
    advance(lexer);
    
    if (getCurrentChar(lexer) == '\\') {
        buffer[bufIndex++] = getCurrentChar(lexer);
        advance(lexer);
        if (getCurrentChar(lexer) != '\0') {
            buffer[bufIndex++] = getCurrentChar(lexer);
            advance(lexer);
        }
    } else {
        buffer[bufIndex++] = getCurrentChar(lexer);
        advance(lexer);
    }
    
    if (getCurrentChar(lexer) == '\'') {
        buffer[bufIndex++] = '\'';
        advance(lexer);
    } else {
        reportError(lexer, "Unterminated character literal");
    }
    buffer[bufIndex] = '\0';
}

void reportError(Lexer *lexer, const char *message) {
    fprintf(stderr, "Error at Line %d, Column %d: %s\n",
            lexer->lineNumber, lexer->columnNumber, message);
    lexer->errorCount++;
}

void reportWarning(Lexer *lexer, const char *message) {
    fprintf(stderr, "Warning at Line %d, Column %d: %s\n",
            lexer->lineNumber, lexer->columnNumber, message);
    lexer->warningCount++;
}

void tokenize(Lexer *lexer) {
    char buffer[MAX_LEXEME_LEN];
    
    while (getCurrentChar(lexer) != '\0') {
        skipWhitespace(lexer);
        
        if (getCurrentChar(lexer) == '\0') break;
        
        // Handle comments
        if (getCurrentChar(lexer) == '/' && 
            (peekChar(lexer, 1) == '/' || peekChar(lexer, 1) == '*')) {
            skipComment(lexer);
            continue;
        }
        
        int startLine = lexer->lineNumber;
        int startCol = lexer->columnNumber;
        
        // Numbers
        if (isdigit(getCurrentChar(lexer))) {
            scanNumber(lexer, buffer);
            int value = atoi(buffer);
            addToken(&lexer->tokenList, TOKEN_NUM, buffer, startLine, startCol, value);
        }
        // Strings
        else if (getCurrentChar(lexer) == '"') {
            scanString(lexer, buffer);
            addToken(&lexer->tokenList, TOKEN_STRING, buffer, startLine, startCol, 0);
        }
        // Character literals
        else if (getCurrentChar(lexer) == '\'') {
            scanCharLiteral(lexer, buffer);
            addToken(&lexer->tokenList, TOKEN_CHAR_LIT, buffer, startLine, startCol, 0);
        }
        // Identifiers and Keywords
        else if (isalpha(getCurrentChar(lexer)) || getCurrentChar(lexer) == '_') {
            scanIdentifier(lexer, buffer);
            if (isKeyword(buffer)) {
                TokenType keywordType = getKeywordType(buffer);
                addToken(&lexer->tokenList, keywordType, buffer, startLine, startCol, 0);
                addSymbol(&lexer->symbolTable, buffer, SYMBOL_KEYWORD, "keyword", 0, startLine);
            } else {
                addToken(&lexer->tokenList, TOKEN_ID, buffer, startLine, startCol, 0);
                if (lookupSymbol(&lexer->symbolTable, buffer) == NULL) {
                    addSymbol(&lexer->symbolTable, buffer, SYMBOL_VARIABLE, "unknown", 0, startLine);
                } else {
                    updateSymbolUsage(&lexer->symbolTable, buffer);
                }
            }
        }
        // Operators and Delimiters
        else {
            char c = getCurrentChar(lexer);
            buffer[0] = c;
            buffer[1] = '\0';
            
            switch (c) {
                case '+':
                    if (peekChar(lexer, 1) == '+') {
                        advance(lexer);
                        strcpy(buffer, "++");
                        addToken(&lexer->tokenList, TOKEN_INC, buffer, startLine, startCol, 0);
                    } else {
                        addToken(&lexer->tokenList, TOKEN_PLUS, buffer, startLine, startCol, 0);
                    }
                    advance(lexer);
                    break;
                    
                case '-':
                    if (peekChar(lexer, 1) == '-') {
                        advance(lexer);
                        strcpy(buffer, "--");
                        addToken(&lexer->tokenList, TOKEN_DEC, buffer, startLine, startCol, 0);
                    } else if (peekChar(lexer, 1) == '>') {
                        advance(lexer);
                        strcpy(buffer, "->");
                        addToken(&lexer->tokenList, TOKEN_ARROW, buffer, startLine, startCol, 0);
                    } else {
                        addToken(&lexer->tokenList, TOKEN_MINUS, buffer, startLine, startCol, 0);
                    }
                    advance(lexer);
                    break;
                    
                case '*':
                    addToken(&lexer->tokenList, TOKEN_MUL, buffer, startLine, startCol, 0);
                    advance(lexer);
                    break;
                    
                case '/':
                    addToken(&lexer->tokenList, TOKEN_DIV, buffer, startLine, startCol, 0);
                    advance(lexer);
                    break;
                    
                case '%':
                    addToken(&lexer->tokenList, TOKEN_MOD, buffer, startLine, startCol, 0);
                    advance(lexer);
                    break;
                    
                case '=':
                    if (peekChar(lexer, 1) == '=') {
                        advance(lexer);
                        strcpy(buffer, "==");
                        addToken(&lexer->tokenList, TOKEN_EQ, buffer, startLine, startCol, 0);
                    } else {
                        addToken(&lexer->tokenList, TOKEN_ASSIGN, buffer, startLine, startCol, 0);
                    }
                    advance(lexer);
                    break;
                    
                case '!':
                    if (peekChar(lexer, 1) == '=') {
                        advance(lexer);
                        strcpy(buffer, "!=");
                        addToken(&lexer->tokenList, TOKEN_NE, buffer, startLine, startCol, 0);
                    } else {
                        addToken(&lexer->tokenList, TOKEN_NOT, buffer, startLine, startCol, 0);
                    }
                    advance(lexer);
                    break;
                    
                case '<':
                    if (peekChar(lexer, 1) == '=') {
                        advance(lexer);
                        strcpy(buffer, "<=");
                        addToken(&lexer->tokenList, TOKEN_LE, buffer, startLine, startCol, 0);
                    } else if (peekChar(lexer, 1) == '<') {
                        advance(lexer);
                        strcpy(buffer, "<<");
                        addToken(&lexer->tokenList, TOKEN_LSHIFT, buffer, startLine, startCol, 0);
                    } else {
                        addToken(&lexer->tokenList, TOKEN_LT, buffer, startLine, startCol, 0);
                    }
                    advance(lexer);
                    break;
                    
                case '>':
                    if (peekChar(lexer, 1) == '=') {
                        advance(lexer);
                        strcpy(buffer, ">=");
                        addToken(&lexer->tokenList, TOKEN_GE, buffer, startLine, startCol, 0);
                    } else if (peekChar(lexer, 1) == '>') {
                        advance(lexer);
                        strcpy(buffer, ">>");
                        addToken(&lexer->tokenList, TOKEN_RSHIFT, buffer, startLine, startCol, 0);
                    } else {
                        addToken(&lexer->tokenList, TOKEN_GT, buffer, startLine, startCol, 0);
                    }
                    advance(lexer);
                    break;
                    
                case '&':
                    if (peekChar(lexer, 1) == '&') {
                        advance(lexer);
                        strcpy(buffer, "&&");
                        addToken(&lexer->tokenList, TOKEN_AND, buffer, startLine, startCol, 0);
                    } else {
                        addToken(&lexer->tokenList, TOKEN_BITAND, buffer, startLine, startCol, 0);
                    }
                    advance(lexer);
                    break;
                    
                case '|':
                    if (peekChar(lexer, 1) == '|') {
                        advance(lexer);
                        strcpy(buffer, "||");
                        addToken(&lexer->tokenList, TOKEN_OR, buffer, startLine, startCol, 0);
                    } else {
                        addToken(&lexer->tokenList, TOKEN_BITOR, buffer, startLine, startCol, 0);
                    }
                    advance(lexer);
                    break;
                    
                case '^':
                    addToken(&lexer->tokenList, TOKEN_BITXOR, buffer, startLine, startCol, 0);
                    advance(lexer);
                    break;
                    
                case '(':
                    addToken(&lexer->tokenList, TOKEN_LPAREN, buffer, startLine, startCol, 0);
                    advance(lexer);
                    break;
                    
                case ')':
                    addToken(&lexer->tokenList, TOKEN_RPAREN, buffer, startLine, startCol, 0);
                    advance(lexer);
                    break;
                    
                case '{':
                    addToken(&lexer->tokenList, TOKEN_LBRACE, buffer, startLine, startCol, 0);
                    advance(lexer);
                    break;
                    
                case '}':
                    addToken(&lexer->tokenList, TOKEN_RBRACE, buffer, startLine, startCol, 0);
                    advance(lexer);
                    break;
                    
                case '[':
                    addToken(&lexer->tokenList, TOKEN_LBRACKET, buffer, startLine, startCol, 0);
                    advance(lexer);
                    break;
                    
                case ']':
                    addToken(&lexer->tokenList, TOKEN_RBRACKET, buffer, startLine, startCol, 0);
                    advance(lexer);
                    break;
                    
                case ';':
                    addToken(&lexer->tokenList, TOKEN_SEMICOLON, buffer, startLine, startCol, 0);
                    advance(lexer);
                    break;
                    
                case ',':
                    addToken(&lexer->tokenList, TOKEN_COMMA, buffer, startLine, startCol, 0);
                    advance(lexer);
                    break;
                    
                case '.':
                    addToken(&lexer->tokenList, TOKEN_DOT, buffer, startLine, startCol, 0);
                    advance(lexer);
                    break;
                    
                case ':':
                    addToken(&lexer->tokenList, TOKEN_COLON, buffer, startLine, startCol, 0);
                    advance(lexer);
                    break;
                    
                default:
                    reportError(lexer, "Unknown character");
                    advance(lexer);
                    break;
            }
        }
    }
    
    addToken(&lexer->tokenList, TOKEN_EOF, "EOF", lexer->lineNumber, lexer->columnNumber, 0);
}

void analyzeLexer(Lexer *lexer) {
    printf("\n========== LEXICAL ANALYSIS REPORT ==========\n");
    printf("Total Tokens: %d\n", lexer->tokenList.count);
    printf("Total Symbols: %d\n", lexer->symbolTable.count);
    printf("Errors: %d\n", lexer->errorCount);
    printf("Warnings: %d\n", lexer->warningCount);
    printf("===========================================\n\n");
}