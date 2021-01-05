#ifndef SEA_VM_H
#define SEA_VM_H
#include "value.h"
#include "utils/int_array.h"
#include "utils/value_array.h"

typedef enum{
    OP_ASSIGN,
    OP_BITWISE_OR,
    OP_BITWISE_XOR,
    OP_BITWISE_AND,
    OP_EQUAL,
    OP_NOT_EQUAL,
    OP_LESS,
    OP_GREATER,
    OP_LESS_EQUAL,
    OP_GREAER_EQUAL,
    OP_LEFT_SHIFT,
    OP_RIGHT_SHIFT,
    OP_ADD,
    OP_MINUS,
    OP_DIVIDE,
    OP_MULTIPLY,
    OP_MODULO,
    OP_U_MINUS,
    OP_NOT,
    OP_BITWISE_NOT,
    OP_PRINT,
    OP_READ_INT
} OpCode;

typedef struct{
    ValueArray* valueStack;
} VM;

void initVm(VM* vm, ValueArray* valueStack);
void freeVm(VM* vm);

void run(VM* vm, OpCode code);

#endif
