#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
#include "decl.h"
#include "data.h"
typedef enum{
    ASSIGN = 0,
    BITWISE_OR = 1,
    BITWISE_XOR = 2,
    BITWISE_AND = 3,
    EQUAL = 4,
    NOT_EQUAL = 4,
    LESS = 5,
    GREATER = 5,
    LESS_EQUAL = 5,
    GREATER_EQUAL = 5,
    LEFT_SHIFT = 6,
    RIGHT_SHIFT  = 6,
    PLUS = 7,
    MINUS = 7,
    MULTIPLY = 8,
    DIVIDE = 8,
    MODULO = 8,
    U_MINUS = 9,
    U_NOT = 9,
    BITWISE_NOT = 9
} OpPrec;

typedef void (*ParseFun)();


OpPrec precendence[] = {
    [TOKEN_LESS] = LESS,
    [TOKEN_LESS_EQUAL] = LESS_EQUAL,
    [TOKEN_GREATER] = GREATER,
    [TOKEN_GREATER_EQUAL] = GREATER_EQUAL,
    [TOKEN_PLUS] = PLUS,
    [TOKEN_MINUS] = MINUS,
    [TOKEN_STAR] = MULTIPLY,
    [TOKEN_SLASH] = DIVIDE,
    [TOKEN_MODULO] = MODULO,

    [TOKEN_EQUAL] = ASSIGN,
    [TOKEN_EQUAL_EQUAL] = EQUAL,
    [TOKEN_BANG] = U_NOT,
    [TOKEN_BANG_EQUAL] = NOT_EQUAL,
    [TOKEN_U_MINUS] = U_MINUS,
    [TOKEN_BITWISE_AND] = BITWISE_AND,
    [TOKEN_BITWISE_OR] = BITWISE_OR,
    [TOKEN_BITWISE_XOR] = BITWISE_XOR,
    [TOKEN_BITWISE_NOT] = BITWISE_NOT,
};

static void intNumber(){
    Value intValue = {
        INTEGER,
        {
            .iNumber = currentToken.value.number
        }
    };
   emit2(OP_READ_INT, pushValue(&(compiledChunk.constants), intValue));
}

static void booleanOp(){
    emit(match(TOKEN_TRUE) ? OP_TRUE : OP_FALSE);
}
static void arithmeticOp(){
    if(opStack.count == 0){
        pushIntArray(&opStack, currentToken.type);
    }else{
        // here we checking for precedence of op on stack >= precedence of current o
        while(!match(TOKEN_OPEN_PAREN) && (precendence[peekIntArray(&opStack)] + 1 > precendence[currentToken.type])){
            emit(popIntArray(&opStack));
        }
        pushIntArray(&opStack, currentToken.type);
    }
}


static void identifier(){
    int index = findIdentifier(currentToken.value.string, currentToken.length);
    if(index > -1){
        emit2(OP_TABLE_GET, index);
    }else{
        printf("Undeclared identifier\n");
        exit(1);

    }
    /*printf("Identifier is undefined!\n");*/
}


static void openParen(){

    pushIntArray(&opStack, TOKEN_OPEN_PAREN);

}
static void closeParen(){
    while(peekIntArray(&opStack) != TOKEN_OPEN_PAREN && opStack.count != 0){
        emit(popIntArray(&opStack));
    }
    popIntArray(&opStack);
}

ParseFun parse[] = {
    [TOKEN_EQUAL] = NULL,
    [TOKEN_EQUAL_EQUAL] = arithmeticOp,
    [TOKEN_BANG_EQUAL] = arithmeticOp,
    [TOKEN_LESS] = arithmeticOp,
    [TOKEN_GREATER] = arithmeticOp,
    [TOKEN_LESS_EQUAL] = arithmeticOp,
    [TOKEN_GREATER_EQUAL] = arithmeticOp,
    [TOKEN_PLUS] = arithmeticOp,
    [TOKEN_MINUS] = arithmeticOp,
    [TOKEN_SLASH] = arithmeticOp,
    [TOKEN_STAR] = arithmeticOp,
    [TOKEN_MODULO] = arithmeticOp,
    [TOKEN_BITWISE_AND] = arithmeticOp,
    [TOKEN_BITWISE_OR] = arithmeticOp,
    [TOKEN_BITWISE_XOR] = arithmeticOp,
    [TOKEN_U_MINUS] = arithmeticOp,
    [TOKEN_BANG] = arithmeticOp,
    [TOKEN_BITWISE_NOT] = arithmeticOp,
    [TOKEN_OPEN_PAREN] = openParen,
    [TOKEN_CLOSE_PAREN] = closeParen,
    [TOKEN_INTEGER] = intNumber,
    [TOKEN_LEFT_SHIFT] = arithmeticOp,
    [TOKEN_RIGHT_SHIFT] = arithmeticOp,
    [TOKEN_TRUE] = booleanOp,
    [TOKEN_FALSE] = booleanOp,
    [TOKEN_IDENTIFIER] = identifier
};


void expression(){
    wrongm(TOKEN_SEMICOLON, "expression expected but got ';'");
    while(!m_semi()){
        parse[currentToken.type]();
        wrongm(TOKEN_EOF, "; expected but file ended");
        scan_into();
    }
    while(opStack.count > 0){
        emit(popIntArray(&opStack));
    }

}

