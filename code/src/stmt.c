#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "defs.h"
#include "decl.h"
#include "data.h"

static void printStatement(){
   scan_into(); 
   expression();
   emit(OP_PRINT);
}


static void varDeclaration(){
    scan_into();
    matchr(TOKEN_IDENTIFIER, "Identifier");
    // there is a identifier
    int length = currentToken.length;
    const char* identifierName = currentToken.value.string; 
    scan_into();
    if(m_semi()){
        // declare global with undefined value
        
        char *chars = malloc(length + 1);
        memcpy(chars, identifierName, length);
        chars[length] = '\0';
        ObjString* string = allocateString(chars, length);
        Value tempValue;
        tempValue.type = STRING;
        tempValue.as.obj = (Obj*)string;
        emit2(OP_TABLE_SET_UNDEFINED, pushValue(&(compiledChunk.constants), tempValue));

    }else if(matchs(TOKEN_EQUAL)){
        expression();
        // declare global with a value
        char *chars = malloc(length + 1);
        memcpy(chars, identifierName, length);
        chars[length] = '\0';
        ObjString* string = allocateString(chars, length);
        Value tempValue;
        tempValue.type = STRING;
        tempValue.as.obj = (Obj*)string;
        emit2(OP_TABLE_SET, pushValue(&(compiledChunk.constants), tempValue));
    }
}

static void assignmentStatement(){
    // token is a identifier

    int length = currentToken.length;
    const char* identifierName = currentToken.value.string;
    scan_into();
    matchsr(TOKEN_EQUAL, "=");
    expression();
    // Assign value to identifier;
    int index = findIdentifier(identifierName, length);
    if(index > -1){
        // identifier is declared
        emit2(OP_TABLE_SET, index);
    }
}

void statements(){
    scan_into();
    while(!m_eof()){
        switch(currentToken.type){
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
                printf("Syntax error :\n");
                return;
        }
    }
    // end of file
    emit(OP_EOF);
}
