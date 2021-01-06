#ifndef SEA_COMPILER_H
#define SEA_COMPILER_H

#include "utils/int_array.h"
#include "utils/value_array.h"
#include "vm.h"
typedef struct{
    IntArray* vmCode;
    ValueArray* constants;
} CompiledChunk;
CompiledChunk compile(const char *buffer);

#endif
