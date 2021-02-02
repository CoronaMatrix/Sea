#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
#include "decl.h"
#include "data.h"
int match(int tokenType){
    return currentToken.type == tokenType;
}

int matchs(int tokenType){
    if(match(tokenType)){
        currentToken = scanToken();
        return 1;
    }
    return 0;
}


int matchsr(int tokenType, const char* whatMsg){
    if(!matchs(tokenType)){
        printf("Syntax error: %s expected at line %d.\n", whatMsg, currentToken.line);
        exit(1);
    }
    return 1;
}

int matchr(int tokenType, const char *whatMsg){

    if(!match(tokenType)){
        printf("Syntax error: %s expected at line %d\n", whatMsg, currentToken.line);
        exit(1);
    }
    return 1;

}

void wrongm(int tokenType, const char *whatMsg){

    if(currentToken.type == tokenType){
        printf(" Syntax error: %s unexpected at line %d\n", whatMsg, currentToken.line);
        exit(1);
    }
}

void scan_into(){
    currentToken = scanToken();
}

int m_semi(){
    return currentToken.type == TOKEN_SEMICOLON;
}
int m_eof(){
    return currentToken.type == TOKEN_EOF;
}
