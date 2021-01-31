#include "object.h"
#include "stdlib.h"
#include "table.h"
#include <stdint.h>

static uint32_t hashString(const char *key, int length){
    
    uint32_t hash = 2166136261u;
    for(int i = 0; i < length; i++){
        hash ^= key[i];
        hash *= 16777619;
    }
    return hash;
}

ObjString* allocateString(char *chars, int length){
    uint32_t hash = hashString(chars, length);
    ObjString* string = malloc(sizeof(ObjString));
    string->chars = chars;
    string->dist = GOLD_NUMBER;
    string->length = length;
    Obj obj = {
       OBJ_STRING 
    };
    string->obj = obj ;
    return string;
}


 /*static uint32_t hashString(const char* key, int length){*/
  /*1     uint32_t hash = 2166136261u;*/
  /*2     for(int i = 0; i < length; i++){*/
  /*3         hash ^= key[i];*/
  /*4         hash *= 16777619;*/
  /*5     }*/
  /*6      */
  /*7     return hash;*/
  /*8 }*/

