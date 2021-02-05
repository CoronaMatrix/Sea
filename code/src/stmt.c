#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "defs.h"
#include "decl.h"
#include "data.h"


static void call_statement();
static void printStatement(){
   scan_into(); 
   expression();
   emit(OP_PRINT, 0);
}


static void varDeclaration(){
    scan_into();
    matchr(TOKEN_IDENTIFIER, "Identifier");
    // there is a identifier
    Symbol symbol = {
        currentToken.value.string,
        currentToken.length,
        scopeDepth
    };
    scan_into();
    int undefined= 0;
    if(m_semi()){
        undefined = 1;

    }else if(matchs(TOKEN_EQUAL)){
        expression();
    }else{
        printf("Syntax error: Wrong way to initialize variables\n");
        exit(1);
    }

    if(undefined){
        emit(OP_NIL, 0);

    }
    if(scopeDepth < 0){
        Value globalVar;
        symbol.slotNumber = pushValue(&(compiledChunk.constants), globalVar);
        if(addSymbol(&symTable, &symbol) < 0){
            printf("Identifier already defined\n");
            exit(1);
        }
        emit2(OP_GLOBAL_SET, symbol.slotNumber, 0);
    }else{
        // for local
        symbol.slotNumber = slotNumber;
        if(addSymbol(&symTable, &symbol) < 0){
            printf("Identifier already defined\n");
            exit(1);
        }
        emit(OP_LOCAL_SET, 0);
    }

}

static void assignmentStatement(){
    int slot = getSymbol(&symTable, scopeDepth < 0 ? 1 : -1, currentToken.value.string, currentToken.length);
    if(slot < 0){
        printf("Undeclared identifier\n");
        exit(1);
    }
    scan_into();


    matchsr(TOKEN_EQUAL, "'='");
    expression();
    emit2(symTable.isGlobal ? OP_GLOBAL_SET : OP_UPDATE_LOCAL, slot, 0);
}

static void error(){
    printf("Syntax error: \n");
    exit(1);
}

// TODO- take look at this method of parsing statements and if necessary change the architecture
static void block(){
    scan_into();
    int prevCount = symTable.count;
    int prevSlotNumber = slotNumber;
    int prevConstsCount = compiledChunk.constants.count;
    /*printf("prevSlotNumber - %d\n", prevSlotNumber);*/
    while(!match(TOKEN_CLOSE_BRACE)){
        wrongm(TOKEN_EOF, "expected '}' but got end of line");
        call_statement();
    }
    // deleting locals from localSymTable
    emit2(OP_LEAVE, prevCount, 0);
    symTable.count = prevCount;
    compiledChunk.constants.count = prevConstsCount;
    slotNumber = prevSlotNumber;

    /*printf("currentSlotNumber - %d\n", slotNumber);*/

    scan_into();
}

static void call_statement(){

    switch(currentToken.type){
        case TOKEN_OPEN_BRACE:
            scopeDepth++;
            block();
            scopeDepth--;

            break;
        case TOKEN_PRINT:
             printStatement();
             matchsr(TOKEN_SEMICOLON, ";");
             break;
        case TOKEN_LET:
             varDeclaration();
             matchsr(TOKEN_SEMICOLON, ";");
             break;
        case TOKEN_IDENTIFIER:
             assignmentStatement();
             matchsr(TOKEN_SEMICOLON, ";");
             break;
        default:
             error();
             break;
    }
}




void statements(){
    scan_into();
    while(!m_eof()){
        call_statement();
    }
    // end of file
    emit(OP_EOF, 0);
}
