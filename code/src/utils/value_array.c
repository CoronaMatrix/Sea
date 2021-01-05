#include <bits/stdint-uintn.h>
#include <stdio.h>
#include <stdlib.h>
#include "value_array.h"
#include "../value.h"

static const int GROW_FACTOR = 2;
void initValueArray(ValueArray *valueArray, int initialCapacity){

    valueArray->capacity = initialCapacity;
    valueArray->count = 0;
    valueArray->values = malloc(initialCapacity * sizeof(Value));

}

void freeValueArray(ValueArray *valueArray){
    free(valueArray->values);
}

uint32_t pushValue(ValueArray *valueArray, Value value){
    
    if(valueArray->capacity < valueArray->count + 1){
        valueArray->capacity = valueArray->capacity * GROW_FACTOR;
        Value* tempStack = malloc(sizeof(Value) * valueArray->capacity);
        free(valueArray->values);
        valueArray->values = tempStack;
    }
    valueArray->values[valueArray->count++] = value;
    return valueArray->count - 1;
}

Value popValue(ValueArray* valueArray){
    if(valueArray->count > 0){
        return valueArray->values[--valueArray->count];
    }
    printf("ValueArray is empty\n");
    Value value;
    return value;
}
