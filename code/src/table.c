#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "defs.h"
#include "decl.h"
#define MAX_LOAD_FACTOR 0.75
static const int GROW_FACTOR = 2;

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
    /*printf("adjustCapacity - %d\n", capacity);*/
    
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
        printf("TableSet Error- key is null\n");
    }
    
    if(table->count + 1 > table->capacity * MAX_LOAD_FACTOR){
        adjustCapacity(table, table->capacity < 8 ? 8 : table->capacity*GROW_FACTOR);
    }


    Entry* entry = &(table->entries[key->hash % (table->capacity)]);
    printf("key - %s, mod: %d\n", key->chars, key->hash % table->capacity);
    if(entry->key == NULL){
        printf("null\n");
        entry->key = key;
        entry->value = *value;
        key->dist = key->hash % table->capacity;
        table->count++;
        return TRUE;
     }


    for(int i = 0, j = table->capacity - 1; i < j; i++, j--){
        if((table->entries + i)->key == NULL){

            key->dist = i;
            (table->entries + i)->key = key;
            (table->entries + i)->value = *value;
            table->count++;
            return TRUE;
        }

        if((table->entries + j)->key == NULL){

            key->dist = j;
            (table->entries + j)->key = key;
            (table->entries + j)->value = *value;
            table->count++;
            return TRUE;
        }
    }
    return FALSE;

}

Bool tableGet(Table *table, ObjString *key, Value* value){
    if(key->dist == GOLD_NUMBER){
        // key never set
        printf("TableGet Error- key{%s} is not present\n", key->chars);
        return FALSE;
    }
    Entry *entry = &(table->entries[key->dist]);
    

    *value = entry->value;
    return TRUE;
}

void tableUpdate(Table* table, ObjString *key, Value *value){
    Entry *entry = table->entries  + key->dist;
    entry->value = *value;
}

// It is caller responsibility to delete allocated memory of key and value if it is a obj

Bool tableDelete(Table *table, ObjString *key){
    if(key->dist == GOLD_NUMBER || !table->count){
        // key never set present
        printf("TableDelete Error- key is not present\n");
        return FALSE;
    }
    Entry* entry = &(table->entries[key->dist]);
    entry->key = NULL;
    /*if(entry->value.type == STRING){*/
       /*free(((ObjString*)(entry->value.as.obj))->chars); */
    /*}*/
    table->count--;
    key->dist = GOLD_NUMBER;
    return TRUE;

}

void debugTable(Table* table, int printDist){
    if(table->count){
        for(int i = 0; i < table->capacity; i++){
            Entry *entry = table->entries+i;
            if(entry->key != NULL){
                if(printDist){
                    printf("--dist: %d -- ", entry->key->dist);
                }
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
                        break;
                    case UNDEFINED:
                        printf("V: %s}\n", "undefined");
                        break;
                }
            }else{
                printf("empty slot\n");
            }
        }
    }
}


