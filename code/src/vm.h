#ifndef SEA_VM_H
#define SEA_VM_H
#include "value.h"
#include "utils/op_stack.h"
#include "utils/value_stack.h"

typedef enum{
    OP_ADD,
    OP_MINUS,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_PRINT,
    OP_MODULO
} OpCode;

typedef struct{
    OpStack* opStack;
    ValueStack* valueStack;

} VM;

void initVm(VM* vm, OpStack* opStack, ValueStack* valueStack);
void freeVm(VM* vm);

void run(VM* vm, OpCode code);

#endif
