#include "compiler.h"
#include "scanner.h"
#include "utils/op_stack.h"
#include "utils/value_stack.h"
#include "value.h"
#include "vm.h"
#include <stdint.h>
#include <stdio.h>

// TODO: remove all functions for parsing expressions [remove repeated code]

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
    U_NOT = 9
} OpPrec;

typedef void (*ParseFun)();

/*OpPrec precendence[] = {LESS, BITWISE_OR, BITWISE_XOR, BITWISE_AND, GREATER, LESS_EQUAL, GREATER_EQUAL, PLUS, MINUS, DIVIDE, MULTIPLY, MODULO, U_MINUS};*/

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
    
};

Token currentToken;

ValueStack valueStack;
OpStack opStack;
VM vm;

static void intNumber(){
    Value intValue;
    intValue.type = INTEGER;
    intValue.as.iNumber = currentToken.value.number;
    pushValue(&valueStack, intValue);
}

static void floatNumber(){

}

static void string(){

}

static void unaryOp(){
    if(opStack.count == 0){
        pushOp(&opStack, currentToken.type);
    }else{

        while(peekOp(&opStack) != TOKEN_OPEN_PAREN && (precendence[peekOp(&opStack)] > precendence[currentToken.type])){
            run(&vm, popOp(&opStack));
        }
        pushOp(&opStack, currentToken.type);
    }
}

static void binaryOp(){
    if(opStack.count == 0){
        pushOp(&opStack, currentToken.type);
    }else{
        while(peekOp(&opStack) != TOKEN_OPEN_PAREN && (precendence[peekOp(&opStack)] > precendence[currentToken.type])){
            run(&vm, popOp(&opStack));
        }
        pushOp(&opStack, currentToken.type);
    }
}

static void relationalOp(){

    if(opStack.count == 0){
        pushOp(&opStack, currentToken.type);
    }else{
        
        while(peekOp(&opStack) != TOKEN_OPEN_PAREN && (precendence[peekOp(&opStack)] > precendence[currentToken.type])){
            run(&vm, popOp(&opStack));
        }
        pushOp(&opStack, currentToken.type);
    }

}

static void equalityOp(){

    if(opStack.count == 0){
        pushOp(&opStack, currentToken.type);
    }else{
        
        while(peekOp(&opStack) != TOKEN_OPEN_PAREN && (precendence[peekOp(&opStack)] > precendence[currentToken.type])){
            run(&vm, popOp(&opStack));
        }
        pushOp(&opStack, currentToken.type);
    }

}

static void bitwiseOp(){

    if(opStack.count == 0){
        pushOp(&opStack, currentToken.type);
    }else{
        while(peekOp(&opStack) != TOKEN_OPEN_PAREN && (precendence[peekOp(&opStack)] > precendence[currentToken.type])){
            run(&vm, popOp(&opStack));
        }
        pushOp(&opStack, currentToken.type);
    }

}

static void shiftOp(){

    if(opStack.count == 0){
        pushOp(&opStack, currentToken.type);
    }else{
        while(peekOp(&opStack) != TOKEN_OPEN_PAREN && (precendence[peekOp(&opStack)] > precendence[currentToken.type])){
            run(&vm, popOp(&opStack));
        }
        pushOp(&opStack, currentToken.type);
    }
}

static void openParen(){

    pushOp(&opStack, TOKEN_OPEN_PAREN);

}
static void closeParen(){
    while(peekOp(&opStack) != TOKEN_OPEN_PAREN && opStack.count != 0){
        run(&vm, popOp(&opStack));
    }
    popOp(&opStack);
}

ParseFun parse[] = {
    [TOKEN_EQUAL] = NULL,
    [TOKEN_EQUAL_EQUAL] = equalityOp,
    [TOKEN_BANG_EQUAL] = equalityOp,
    [TOKEN_LESS] = relationalOp,
    [TOKEN_GREATER] = relationalOp,
    [TOKEN_LESS_EQUAL] = relationalOp,
    [TOKEN_GREATER_EQUAL] = relationalOp,
    [TOKEN_PLUS] = binaryOp,
    [TOKEN_MINUS] = binaryOp,
    [TOKEN_SLASH] = binaryOp,
    [TOKEN_STAR] = binaryOp,
    [TOKEN_MODULO] = binaryOp,
    [TOKEN_BITWISE_AND] = bitwiseOp,
    [TOKEN_BITWISE_OR] = bitwiseOp,
    [TOKEN_BITWISE_XOR] = bitwiseOp,
    [TOKEN_U_MINUS] = unaryOp,
    [TOKEN_BANG] = unaryOp,
    [TOKEN_OPEN_PAREN] = openParen,
    [TOKEN_CLOSE_PAREN] = closeParen,
    [TOKEN_INTEGER] = intNumber,
    [TOKEN_LEFT_SHIFT] = shiftOp,
    [TOKEN_RIGHT_SHIFT] = shiftOp,
};

void expression(){
    currentToken = scanToken();
    while(currentToken.type != TOKEN_SEMICOLON){
        parse[currentToken.type]();
        currentToken = scanToken();

    }
    while(opStack.count > 0){
        run(&vm, popOp(&opStack));
    }

    run(&vm, OP_PRINT);
}

void compile(const char *buffer){

    initScanner(buffer);
    initVm(&vm, &opStack, &valueStack);
    expression();
    freeVm(&vm);

}
