
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "defs.h"
#include "decl.h"
#include "data.h"
 
// initial vm slot number
int slotNumber = -1;
int scopeDepth = -1;
uint32_t varCount = 0;




static int8_t slotChangeFactor[] = {
    [OP_ASSIGN] = 0,
    [OP_BITWISE_OR] = -1,
    [OP_BITWISE_XOR] = -1,
    [OP_BITWISE_AND] = -1,
    [OP_BITWISE_NOT] = -1,
    [OP_ADD]  = -1,
    [OP_MINUS] = -1,
    [OP_MULTIPLY] = -1,
    [OP_DIVIDE] = -1,
    [OP_MODULO] = -1,
    [OP_U_MINUS] = 0,
    [OP_NOT] = 0,
    [OP_LESS] = -1,
    [OP_GREAER_EQUAL] = -1,
    [OP_GREATER] =-1,
    [OP_LESS_EQUAL] = -1,
    [OP_EQUAL] = -1,
    [OP_NOT_EQUAL] = -1,
    [OP_LEFT_SHIFT] = -1,
    [OP_RIGHT_SHIFT] = -1,
    [OP_READ_INT] = 1,
    [OP_TABLE_SET] = -1,
    [OP_TABLE_SET_UNDEFINED] = 0,
    [OP_TABLE_UPDATE] = -1,
    [OP_TABLE_GET] = 1,
    [OP_TRUE] = 1,
    [OP_FALSE] = 1,
    [OP_EOF] = 0,
    [OP_PRINT] = -1,
    [OP_LOCAL_GET] = 1,
    [OP_LOCAL_SET] = 0,
};



void emit(uint32_t op, int isIndex){
    if(!isIndex){
        slotNumber += slotChangeFactor[op];
        /*printf("slotNumber - %d\n", slotNumber);*/
    }
    pushIntArray(&(compiledChunk.vmCode), op);
}

void emit2(uint32_t op1, uint32_t op2, int isIndex){

    if(!isIndex){
        slotNumber += slotChangeFactor[op1];
        /*printf("slotNumber - %d\n", slotNumber);*/
    }
    pushIntArray(&(compiledChunk.vmCode), op1);
    pushIntArray(&(compiledChunk.vmCode), op2);
}


void compile(char *buffer){

    initScanner(buffer);
    initIntArray(&opStack, 10);
    initSymbolTable(&localSymTable, 10);
    initSymbolTable(&globalSymTable, 10);
    initIntArray(&(compiledChunk.vmCode), 50);
    initIntArray(&indexes, 10);
    initValueArray(&(compiledChunk.constants), 10);

    statements();

    freeIntArray(&opStack);
    freeSymbolTable(&localSymTable);
    freeIntArray(&indexes);
}

