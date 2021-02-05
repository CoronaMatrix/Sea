#include <stdio.h>
#include <bits/stdint-uintn.h>
#include "../defs.h"
#include "../decl.h"
#include "../data.h"
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
    
    if(getSymbol(symbolTable, symbol->depth < 0 ? 1 : -1, symbol->name, symbol->length) > -1){

        // symbol already present
        return -1;
    }
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

    int index = symbolTable->count -1 ;
    // searching for globals or locals
    int uptoDepth = scopeDepth + 1;
    if(direction > 0){
        // search for globalVar
        index = 0;
        uptoDepth = 0;
    }
    for(; index >= 0 && index < symbolTable->count; index+= direction){
        Symbol *symbol = &(symbolTable->symbols[index]);

        if((symbol->depth + 1) > uptoDepth){
            return - 1;
        }
        if((symbol->length == length) && !memcmp(symbol->name, symbolName, symbol->length)){
            // symbol found
            symbolTable->isGlobal = symbol->depth < 0 ? 1 : 0;

            return symbol->slotNumber;
        }
    }
    return -1;
}

void deleteSymbols(SymbolTable *symbolTable, int count){
   symbolTable->count = count; 
}
