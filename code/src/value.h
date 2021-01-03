#ifndef SEA_VALUE_H
#define SEA_VALUE_H

#include <stdint.h>
typedef enum{
    INTEGER,
    BOOL,
    FLOAT,
} ValueType;

typedef struct{
    ValueType type;
    union{
        float fNumber;
        int iNumber;
    }as;
} Value;

#endif
