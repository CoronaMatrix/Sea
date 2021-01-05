#ifndef SEA_VALUE_STACK_H
#define SEA_VALUE_STACK_H

#include "../value.h"
#include <bits/stdint-uintn.h>

typedef struct{
    int capacity;
    int count;
    Value* values;
} ValueArray;

void initValueArray(ValueArray* valueArray, int initialCapacity);
void freeValueArray(ValueArray* valueArray);
uint32_t pushValue(ValueArray* valueArray, Value value);
Value popValue(ValueArray* valueArray);
#endif
