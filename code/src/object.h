#ifndef SEA_OBJECT_H
#define SEA_OBJECT_H

#include <bits/stdint-uintn.h>
typedef enum{
    OBJ_STRING
} ObjType;

typedef struct{
    ObjType objType;
} Obj;

typedef struct {
    Obj obj;
    int length;
    uint32_t hash;
    int dist;
    char* chars;

} ObjString;

ObjString* allocateString(char* chars, int length);

#endif
