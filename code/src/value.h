#ifndef SEA_VALUE_H
#define SEA_VALUE_H

typedef enum{
    INTEGER,
} ValueType;

typedef struct{
    ValueType type;
    union{
        int iNumber;
    }as;
} Value;

#endif
