#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "op_stack.h"

static const int INITIAL_CAPACITY = 4;
static const int GROW_FACTOR = 2;
void initOpStack(OpStack *opStack){
    opStack->capacity = INITIAL_CAPACITY;
    opStack->count =0;
    opStack->operators = malloc(opStack->capacity);

}

void freeOpStack(OpStack *opStack){
    free(opStack->operators);
    opStack->capacity = 0;
    opStack->count = 0;
}


void pushOp(OpStack *opStack, uint8_t op){
    if(opStack->capacity < opStack->count + 1){
        opStack->capacity = opStack->capacity * GROW_FACTOR;
        uint8_t *tempStack = malloc(opStack->capacity);
        memcpy(tempStack, opStack->operators, opStack->capacity);
        free(opStack->operators);
        opStack->operators = tempStack;

    }

    opStack->operators[opStack->count++] = op;
    
}

uint8_t popOp(OpStack *opStack){
    if(opStack->count > 0){
        return opStack->operators[--opStack->count];
    }
    printf("Stack is empty.\n");
    return 255;
}
