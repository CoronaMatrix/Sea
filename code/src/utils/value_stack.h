#ifndef SEA_VALUE_STACK_H
#define SEA_VALUE_STACK_H

#include "../value.h"

typedef struct{
    int capacity;
    int count;
    Value* values;
} ValueStack;

void initValueStack(ValueStack* valueStack);
void freeValueStack(ValueStack* valueStack);
void pushValue(ValueStack* valueStack, Value value);
Value popValue(ValueStack* valueStack);
#endif
