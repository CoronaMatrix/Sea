#ifndef  SEA_TABLE_H
#define SEA_TABLE_H

#include "value.h"
#include "object.h"

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
bool tableSet(Table* table, ObjString* key, Value* value);
Value tableGet(Table* table, ObjString* key);
bool tableDelete(Table* table, ObjString* key);
void freeTable(Table* table);


#endif
