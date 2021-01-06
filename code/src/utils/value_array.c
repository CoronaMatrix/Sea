#include <bits/stdint-uintn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    
    int oldCapacity = valueArray->capacity;
    if(oldCapacity < valueArray->count + 1){
        valueArray->capacity = oldCapacity * GROW_FACTOR;
        Value* tempArray = malloc(sizeof(Value) * valueArray->capacity);
        memcpy(tempArray, valueArray->values, oldCapacity*sizeof(Value));
        free(valueArray->values);
        valueArray->values = tempArray;
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
