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
    /*printf("prevSlotNumber - %d\n", prevSlotNumber);*/
    while(!match(TOKEN_CLOSE_BRACE)){
        wrongm(TOKEN_EOF, "expected '}' but got end of line");
        call_statement();
    }
    // deleting locals from localSymTable
    emit2(OP_LEAVE, prevCount, 0);
    symTable.count = prevCount;
    slotNumber = prevSlotNumber;

    /*printf("currentSlotNumber - %d\n", slotNumber);*/

    scan_into();
}

static void whileStatement(){

    scan_into();
    matchr(TOKEN_OPEN_PAREN, "(");
    int gotoCode = compiledChunk.vmCode.count;
    printf("goto- %d\n", gotoCode);
    expression();
    emit2(OP_PATCH_JUMP, 0xff, 1);
    int jumpCode = compiledChunk.vmCode.count - 1;
    matchr(TOKEN_OPEN_BRACE, "{");
    block();
    emit2(OP_GOTO, gotoCode, 1);
    compiledChunk.vmCode.values[jumpCode] = (compiledChunk.vmCode.count);

}

static void ifStatement(){
    scan_into();
    matchr(TOKEN_OPEN_PAREN, "(");
    expression();
    emit2(OP_PATCH_JUMP,0xff, 1);
    int jumpCode = compiledChunk.vmCode.count - 1;
    matchr(TOKEN_OPEN_BRACE, "{");
    block();
    if(match(TOKEN_ELSE)){
        scan_into();
        emit2(OP_JUMP, 0xff, 1);
        compiledChunk.vmCode.values[jumpCode] = (compiledChunk.vmCode.count);
        jumpCode = compiledChunk.vmCode.count - 1;
        block();
        compiledChunk.vmCode.values[jumpCode] = (compiledChunk.vmCode.count);
    }else{
        compiledChunk.vmCode.values[jumpCode] = (compiledChunk.vmCode.count);
    }
}

static void call_statement(){

    switch(currentToken.type){
        case TOKEN_IF:
            ifStatement();
            break;
        case TOKEN_WHILE:
            whileStatement();
            break;
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
             printf("error : %d\n", currentToken.value.number);
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
