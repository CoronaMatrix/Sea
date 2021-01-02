#include <stdio.h>
#include <stdlib.h>
#include "value_stack.h"
#include "../value.h"

static const int INITIAL_CAPACITY = 124;
static const int GROW_FACTOR = 2;
void initValueStack(ValueStack *valueStack){

    valueStack->capacity = INITIAL_CAPACITY;
    valueStack->count = 0;
    valueStack->values = malloc(INITIAL_CAPACITY * sizeof(Value));

}

void freeValueStack(ValueStack *valueStack){
    free(valueStack->values);
}

void pushValue(ValueStack *valueStack, Value value){
    
    if(valueStack->capacity < valueStack->count + 1){
        valueStack->capacity = valueStack->capacity * GROW_FACTOR;
        Value* tempStack = malloc(sizeof(Value) * valueStack->capacity);
        free(valueStack->values);
        valueStack->values = tempStack;
    }
    valueStack->values[valueStack->count++] = value;
}

Value popValue(ValueStack* valueStack){
    if(valueStack->count > 0){
        return valueStack->values[--valueStack->count];
    }
    printf("ValueStack is empty\n");
    Value value;
    return value;
}
