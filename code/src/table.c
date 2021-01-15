#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "table.h"
#include "utils/common.h"

#define MAX_LOAD_FACTOR 0.75
static const int GROW_FACTOR = 3;

void initTable(Table* table){
    table->capacity = 0;
    table->count = 0;
    table->entries = NULL;
}


void freeTable(Table *table){
    free(table->entries);
    initTable(table);
}

static void adjustCapacity(Table* table, uint32_t capacity){
    
    Entry* tempEntries = malloc(sizeof(Entry)*capacity);
    for(int i = table->count; i < capacity; i++){
        tempEntries[i].key = NULL;
    }
    memcpy(tempEntries, table->entries, sizeof(Entry)*table->capacity);
    free(table->entries);
    table->entries = tempEntries;
    table->capacity = capacity;
}

Bool tableSet(Table *table, ObjString *key, Value *value){

    if(key == NULL){
        return FALSE;
    }
    
    if(table->count + 1 > table->capacity * MAX_LOAD_FACTOR){
        adjustCapacity(table, table->capacity * GROW_FACTOR);
    }

    Entry* entry = &(table->entries[key->hash % (table->capacity - 1)]);
    if(entry->key == NULL){
        Entry temp = {
            key, *value
        };
        *entry = temp;
        return TRUE;
     }

    if(!(memcmp(entry->key, key, key->length))){
        // keys are same override
        entry->value = *value;
        return TRUE;
    }

    for(int i = 0, j = table->capacity - 1; i < j; i++, j--){
        if(table->entries[i].key == NULL){

            Entry temp = {
                key, *value
            };
            *entry = temp;
            return TRUE;
        }

        if(table->entries[j].key == NULL){
            Entry temp = {
                key, *value
            };
            *entry = temp;
            return TRUE;
        }
    }
    return FALSE;

}
