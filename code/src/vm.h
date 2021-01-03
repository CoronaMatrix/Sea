#ifndef SEA_VM_H
#define SEA_VM_H
#include "value.h"
#include "utils/op_stack.h"
#include "utils/value_stack.h"

typedef enum{
    OP_ADD = 0,
    OP_MINUS = 1,
    OP_DIVIDE = 2,
    OP_MULTIPLY = 3,
    OP_MODULO = 4,
    OP_U_MINUS = 5,
    OP_PRINT = 6
} OpCode;

typedef struct{
    OpStack* opStack;
    ValueStack* valueStack;

} VM;

void initVm(VM* vm, OpStack* opStack, ValueStack* valueStack);
void freeVm(VM* vm);

void run(VM* vm, OpCode code);

#endif
