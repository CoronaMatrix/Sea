// All definations

#ifndef SEA_DEFS_H
#define SEA_DEFS_H
#include <bits/stdint-uintn.h>

typedef enum{
    OBJ_STRING
} ObjType;

typedef struct{
    ObjType objType;
} Obj;

typedef struct {
    Obj obj;
    int length;
    uint32_t hash;
    int dist;
    char* chars;

} ObjString;


typedef enum{
    INTEGER,
    BOOL,
    FLOAT,
    STRING,
    UNDEFINED
} ValueType;

typedef struct{
    ValueType type;
    union{
        float fNumber;
        int iNumber;
        Obj* obj;
    }as;
} Value;

#ifndef GOLD_NUMBER 
#define GOLD_NUMBER 999999999 
#endif

typedef struct{
    ObjString* key;
    Value value;

} Entry;

typedef struct{
    uint32_t count;
    uint32_t capacity;
    Entry* entries;
} Table;
typedef enum{
    TRUE,
    FALSE
} Bool;

typedef struct{
   uint32_t *values;
   int count;
   int capacity;
} IntArray;


typedef struct{
    int capacity;
    int count;
    Value* values;
} ValueArray;


typedef struct{
    const char* name;
    uint32_t length;
    int depth;
    int slotNumber;
} Symbol;

typedef struct{
    int capacity;
    int count;
    int isGlobal;
    Symbol* symbols;
}SymbolTable;

typedef enum{

    TOKEN_EQUAL = 0,
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
    TOKEN_OPEN_BRACE,
    TOKEN_CLOSE_BRACE,
    TOKEN_INTEGER,
    TOKEN_IDENTIFIER,
    TOKEN_SEMICOLON,
    TOKEN_LET,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_PRINT,
    TOKEN_FALSE,
    TOKEN_TRUE,
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

typedef enum{
    OP_ASSIGN = TOKEN_EQUAL,
    OP_BITWISE_OR = TOKEN_BITWISE_OR,
    OP_BITWISE_XOR = TOKEN_BITWISE_XOR,
    OP_BITWISE_AND = TOKEN_BITWISE_AND,
    OP_EQUAL = TOKEN_EQUAL_EQUAL,
    OP_NOT_EQUAL = TOKEN_BANG_EQUAL,
    OP_LESS = TOKEN_LESS,
    OP_GREATER = TOKEN_GREATER,
    OP_LESS_EQUAL = TOKEN_LESS_EQUAL,
    OP_GREAER_EQUAL = TOKEN_GREATER_EQUAL,
    OP_LEFT_SHIFT = TOKEN_LEFT_SHIFT,
    OP_RIGHT_SHIFT = TOKEN_RIGHT_SHIFT,
    OP_ADD = TOKEN_PLUS,
    OP_MINUS = TOKEN_MINUS,
    OP_DIVIDE = TOKEN_SLASH,
    OP_MULTIPLY = TOKEN_STAR,
    OP_MODULO = TOKEN_MODULO,
    OP_U_MINUS = TOKEN_U_MINUS,
    OP_NOT = TOKEN_BANG,
    OP_BITWISE_NOT = TOKEN_BITWISE_NOT,
    OP_PRINT = TOKEN_PRINT,
    OP_READ_INT = TOKEN_INTEGER,
    OP_TRUE = TOKEN_TRUE,
    OP_FALSE = TOKEN_FALSE,
    OP_EOF = TOKEN_EOF,
    OP_LOCAL_SET,
    OP_LOCAL_GET,
    OP_UPDATE_LOCAL,
    OP_NIL,
    OP_PATCH_JUMP,
    OP_JUMP,
    OP_GLOBAL_SET,
    OP_GLOBAL_GET,
    OP_ASSIGN_LOCAL,
    OP_ASSIGN_GLOBAL,
    OP_LEAVE
} OpCode;

typedef struct{
    uint32_t *vmCode;
    ValueArray valueStack;
} VM;

typedef struct{
    IntArray vmCode;
    ValueArray constants;
} CompiledChunk;
#endif
