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
    if(m_semi()){
        if(scopeDepth < 0){
            addSymbol(&globalSymTable, &symbol);
        }else{
            // local
            addSymbol(&localSymTable, &symbol);
        }

    }else if(matchs(TOKEN_EQUAL)){
        expression();
        symbol.slotNumber = slotNumber;
        if(scopeDepth < 0){
            emit2(OP_TABLE_SET, addSymbol(&globalSymTable, &symbol), 0);
        }else{
            addSymbol(&localSymTable, &symbol);
            emit(OP_LOCAL_SET, 0);
            
        }
    }
}

static void assignmentStatement(){
    printf("assignmentStatement\n");
}

static void error(){
    printf("Syntax error: \n");
    exit(1);
}

// TODO- take look at this method of parsing statements and if necessary change the architecture
static void block(){
    scan_into();
    int prevCount = localSymTable.count;
    while(!match(TOKEN_CLOSE_BRACE)){
        wrongm(TOKEN_EOF, "expected '}' but got end of line");
        call_statement();
    }
    // deleting locals from localSymTable
    localSymTable.count = prevCount;
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
