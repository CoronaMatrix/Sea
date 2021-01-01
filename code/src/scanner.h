typedef enum{

    TOKEN_INTEGER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_SLASH,
    TOKEN_STAR,
    TOKEN_MODULO,


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