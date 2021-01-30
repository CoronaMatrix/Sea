#ifndef SEA_VALUE_H
#define SEA_VALUE_H

#include "object.h"
#include <stdint.h>
typedef enum{
    INTEGER,
    BOOL,
    FLOAT,
    STRING,
} ValueType;

typedef struct{
    ValueType type;
    union{
        float fNumber;
        int iNumber;
        Obj* obj;
    }as;
} Value;

#endif
