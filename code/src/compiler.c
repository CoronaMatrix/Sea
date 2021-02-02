
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "defs.h"
#include "decl.h"
#include "data.h"
 

void emit(uint32_t op){
    pushIntArray(&(compiledChunk.vmCode), op);
}

void emit2(uint32_t op1, uint32_t op2){

    pushIntArray(&(compiledChunk.vmCode), op1);
    pushIntArray(&(compiledChunk.vmCode), op2);
}

int findIdentifier(const char* identifierName, int length){

    for(int i = 0; i < compiledChunk.constants.count; i++){
        if(compiledChunk.constants.values[i].type == STRING){
            ObjString* string = (ObjString*)compiledChunk.constants.values[i].as.obj;
            if(!memcmp(identifierName, string->chars, length)){
                return i;
            }
        }
    }
    return -1;
}

void compile(char *buffer){

    initScanner(buffer);
    initIntArray(&opStack, 10);
    
    initIntArray(&(compiledChunk.vmCode), 50);
    initIntArray(&indexes, 10);
    initValueArray(&(compiledChunk.constants), 10);
    statements();
    freeIntArray(&opStack);
    freeIntArray(&indexes);
    free(buffer);
}

