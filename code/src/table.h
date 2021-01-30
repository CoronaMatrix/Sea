#ifndef  SEA_TABLE_H
#define SEA_TABLE_H

#include "value.h"
#include "object.h"
#include "./utils/common.h"
#define GOLD_NUMBER 999999999 

typedef struct{
    ObjString* key;
    Value value;

} Entry;

typedef struct{
    uint32_t count;
    uint32_t capacity;
    Entry* entries;
} Table;

void initTable(Table* table);
Bool tableSet(Table* table, ObjString* key, Value* value);
// returns value in value* pointer
Bool tableGet(Table* table, ObjString* key, Value* value);
Bool tableDelete(Table* table, ObjString* key);
void freeTable(Table* table);
void debugTable(Table* table);


#endif
