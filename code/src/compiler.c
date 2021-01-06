#include "compiler.h"
#include "scanner.h"
#include "utils/int_array.h"
#include "utils/value_array.h"
#include "value.h"
#include "vm.h"
#include <stdint.h>
#include <stdio.h>

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

Token currentToken;

IntArray opStack;
IntArray vmOp;
ValueArray constants;

void emit(uint32_t op){
    pushIntArray(&vmOp, op);
}

void emit2(uint32_t op1, uint32_t op2){

    pushIntArray(&vmOp, op1);
    pushIntArray(&vmOp, op2);
}

static void intNumber(){
    Value intValue = {
        INTEGER,
        {
            .iNumber = currentToken.value.number
        }
    };
   emit2(OP_READ_INT, pushValue(&constants, intValue));
}

static void floatNumber(){

}

static void string(){

}

static void arithmeticOp(){
    if(opStack.count == 0){
        pushIntArray(&opStack, currentToken.type);
    }else{

        while(peekIntArray(&opStack) != TOKEN_OPEN_PAREN && (precendence[peekIntArray(&opStack)] > precendence[currentToken.type])){
            emit(popIntArray(&opStack));
        }
        pushIntArray(&opStack, currentToken.type);
    }
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
};


void expression(){
    currentToken = scanToken();
    while(currentToken.type != TOKEN_SEMICOLON){
        parse[currentToken.type]();
        currentToken = scanToken();

    }
    while(opStack.count > 0){
        emit(popIntArray(&opStack));
    }

    emit(OP_PRINT);

}


CompiledChunk compile(const char *buffer){

    initScanner(buffer);
    initIntArray(&opStack, 30);
    initIntArray(&vmOp, 100);
    initValueArray(&constants, 30);
    expression();
    freeIntArray(&opStack);
    CompiledChunk compiledChunk = {
        &vmOp,
        &constants
    };
    emit(OP_EOF);
    return compiledChunk;
}

