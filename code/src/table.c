#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "object.h"
#include "table.h"
#include "utils/common.h"
#include "value.h"

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
        adjustCapacity(table, (table->capacity > 0 ? table->capacity : 1) * GROW_FACTOR);
        printf("adjust-%d\n", table->capacity);
    }


    Entry* entry = &(table->entries[key->hash % (table->capacity)]);
    if(entry->key == NULL){
        Entry temp = {
            key, *value
        };
        *entry = temp;
        key->dist = 0;
        table->count++;
        return TRUE;
     }

    if(!(memcmp(entry->key, key, key->length))){
        // keys are same override
        entry->value = *value;
        table->count++;
        return TRUE;
    }

    for(int i = 0, j = table->capacity - 1; i < j; i++, j--){
        if(table->entries[i].key == NULL){

            Entry temp = {
                key, *value
            };
            key->dist = i - (key->hash % (table->capacity));

            *entry = temp;
            table->count++;
            return TRUE;
        }

        if(table->entries[j].key == NULL){
            Entry temp = {
                key, *value
            };
            key->dist = j - (key->hash % (table->capacity));
            *entry = temp;
            table->count++;
            return TRUE;
        }
    }
    return FALSE;

}

Bool tableGet(Table *table, ObjString *key, Value* value){
    if(key->dist == GOLD_NUMBER){
        // key never set
        return FALSE;
    }
    Entry *entry = &(table->entries[(key->hash % table->capacity) + key->dist]);
    
    if(entry->key == NULL){
        return FALSE;
    }

    *value = entry->value;
    return TRUE;
}

Bool tableDelete(Table *table, ObjString *key){
    if(key->dist == GOLD_NUMBER || !table->count){
        // key never set present
        return FALSE;
    }
    Entry* entry = &(table->entries[(key->hash % table->capacity) + key->dist]);
    if(entry->key == NULL ){
        // key may be deleted
        return FALSE;
    }
    entry->key = NULL;
    if(entry->value.type == STRING){
       free(((ObjString*)(entry->value.as.obj))->chars); 
    }
    table->count--;
    key->dist = GOLD_NUMBER;
    return TRUE;

}

void debugTable(Table* table){
    if(table->count){
        for(int i = 0; i < table->capacity; i++){
            Entry *entry = table->entries+i;
            if(entry->key != NULL){
                printf("{K: %s, ",entry->key->chars);
                switch (entry->value.type){
                    case INTEGER:
                        printf("V: %d}\n", entry->value.as.iNumber);
                        break;
                    case STRING:
                        printf("V: %s}\n", ((ObjString*)(entry->value.as.obj))->chars);
                        break;
                    case BOOL:
                        printf("V: %s}\n", entry->value.as.iNumber == 0 ? "false" : "true");
                        break;
                    case FLOAT:
                        printf("V: %f}\n", entry->value.as.fNumber);
                }
            }else{
                printf("empty slot\n");
            }
        }
    }
}
