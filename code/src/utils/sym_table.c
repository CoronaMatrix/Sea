#include <stdio.h>
#include <bits/stdint-uintn.h>
#include "../defs.h"
#include "../decl.h"
#include <stdlib.h>
#include <string.h>

void initSymbolTable(SymbolTable *symbolTable, uint32_t initialCapacity){
    symbolTable->capacity = initialCapacity;
    symbolTable->count = 0;
    if(initialCapacity > 0){
        symbolTable->symbols = malloc(initialCapacity * sizeof(Symbol));
    }
}

void freeSymbolTable(SymbolTable *symbolTable){
   free(symbolTable->symbols); 
   initSymbolTable(symbolTable, 0);
}

int addSymbol(SymbolTable *symbolTable, Symbol *symbol){
    int oldCapacity = symbolTable->capacity;
    if(oldCapacity < symbolTable->count + 1){
        Symbol* symbols = malloc(oldCapacity * 2);
        memcpy(symbols, symbolTable->symbols, oldCapacity);
        free(symbolTable->symbols);
        symbolTable->symbols = symbols;
        symbolTable->capacity = oldCapacity * 2;
    }
    symbolTable->symbols[symbolTable->count++] = *symbol;
    return symbolTable->count-1;
}


// 1 for forward direction
// -1 for backword direction
int getSymbol(SymbolTable *symbolTable,int direction, const char* symbolName, int length){

    int index = direction > 0 ? 0 : symbolTable->count-1;
    for(; index >= 0 && index < symbolTable->count; index+= direction){
        Symbol *symbol = &(symbolTable->symbols[index]);
        if((symbol->length == length) && !memcmp(symbol->name, symbolName, symbol->length)){
            // symbol found
            return symbol->slotNumber;
        }
    }
    return -1;
}

void deleteSymbols(SymbolTable *symbolTable, int count){
   symbolTable->count = count; 
}
