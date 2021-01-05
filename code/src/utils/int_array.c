#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "op_stack.h"

static const int GROW_FACTOR = 2;

void initIntArray(IntArray *intArray, int initialCapacity){
    intArray->capacity = initialCapacity;
    intArray->count =0;
    intArray->values = malloc(intArray->capacity);
}

void freeIntArray(IntArray *intArray){
    free(intArray->values);
    intArray->capacity = 0;
    intArray->count = 0;
}

uint32_t peekIntArray(IntArray* intArray){
    return intArray->values[intArray->count - 1];
}

void pushIntArray(IntArray *intArray, uint32_t op){
    if(intArray->capacity < intArray->count + 1){
        intArray->capacity = intArray->capacity * GROW_FACTOR;
        uint32_t *tempStack = malloc(intArray->capacity);
        memcpy(tempStack, intArray->values, intArray->capacity);
        free(intArray->values);
        intArray->values = tempStack;

    }

    intArray->values[intArray->count++] = op;
    
}

uint32_t popIntArray(IntArray *intArray){
    if(intArray->count > 0){
        return intArray->values[--intArray->count];
    }
    printf("Stack is empty.\n");
    return 255;
}
