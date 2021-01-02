#ifndef SEA_OP_STACK_H
#define SEA_OP_STACK_H

#include <stdint.h>
typedef struct{
   uint8_t *operators;
   int count;
   int capacity;
} OpStack;

void initOpStack(OpStack* opStack);
void freeOpStack(OpStack* opStack);
void pushOp(OpStack* opStack, uint8_t op);
uint8_t popOp(OpStack* opStack);

#endif