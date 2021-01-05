#ifndef SEA_OP_STACK_H
#define SEA_OP_STACK_H

#include <stdint.h>
typedef struct{
   uint32_t *values;
   int count;
   int capacity;
} IntArray;

void initIntArray(IntArray* intArray, int initialCapacity);
void freeIntArray(IntArray* intArray);
void pushIntArray(IntArray* intArray, uint32_t intValue);
uint32_t peekIntArray(IntArray* intArray);
uint32_t popIntArray(IntArray* intArray);

#endif
