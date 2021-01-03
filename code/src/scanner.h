#ifndef SEA_SCANNER_H
#define SEA_SCANNER_H
typedef enum{


    TOKEN_PLUS = 0,
    TOKEN_MINUS = 1,
    TOKEN_SLASH = 2,
    TOKEN_STAR = 3,
    TOKEN_MODULO = 4,
    TOKEN_U_MINUS = 5,
    TOKEN_OPEN_PAREN = 6,
    TOKEN_CLOSE_PAREN = 7,
    TOKEN_INTEGER = 8,
    TOKEN_SEMICOLON = 9


} TokenType;

typedef union{

    int number;

} TokenValue;

typedef struct{

    TokenType type;
    int line;
    TokenValue value;

} Token;

void initScanner(const char* buffer);
Token scanToken();

#endif
