#ifndef SEA_SCANNER_H
#define SEA_SCANNER_H
typedef enum{

    TOKEN_EQUAL,
    TOKEN_BITWISE_OR,
    TOKEN_BITWISE_XOR,
    TOKEN_BITWISE_AND,
    TOKEN_EQUAL_EQUAL,
    TOKEN_BANG_EQUAL,
    TOKEN_LESS,
    TOKEN_GREATER,
    TOKEN_LESS_EQUAL,
    TOKEN_GREATER_EQUAL,
    TOKEN_LEFT_SHIFT,
    TOKEN_RIGHT_SHIFT,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_SLASH,
    TOKEN_STAR,
    TOKEN_MODULO,
    TOKEN_U_MINUS,
    TOKEN_BANG,
    TOKEN_BITWISE_NOT,
    TOKEN_OPEN_PAREN,
    TOKEN_CLOSE_PAREN,
    TOKEN_INTEGER,
    TOKEN_IDENTIFIER,
    TOKEN_SEMICOLON,
    TOKEN_LET,
    TOKEN_PRINT,
    TOKEN_EOF

} TokenType;

typedef union{

    int number;
    const char* string;

} TokenValue;

typedef struct{

    TokenType type;
    int line;
    int length;
    TokenValue value;

} Token;

void initScanner(const char* buffer);
Token scanToken();

#endif
