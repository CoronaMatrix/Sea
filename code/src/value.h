#ifndef SEA_VALUE_H
#define SEA_VALUE_H

#include <stdint.h>
typedef enum{
    INTEGER,
    BOOL,
} ValueType;

typedef struct{
    ValueType type;
    union{
        int iNumber;
    }as;
} Value;

#endif
