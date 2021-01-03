#include "compiler.h"
#include "scanner.h"
#include "utils/op_stack.h"
#include "utils/value_stack.h"
#include "value.h"
#include "vm.h"
#include <stdint.h>
#include <stdio.h>

typedef enum{
    PLUS = 0,
    MINUS = 0,
    MULTIPLY = 1,
    DIVIDE = 1,
    MODULO = 1,
    U_MINUS = 2
} OpPrec;

typedef void (*ParseFun)();

OpPrec precendence[] = {PLUS, MINUS, DIVIDE, MULTIPLY, MODULO, U_MINUS};

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

        uint8_t stackOp = peekOp(&opStack);
        if(stackOp != TOKEN_OPEN_PAREN && (precendence[stackOp] > precendence[currentToken.type])){
            run(&vm, popOp(&opStack));
        }else{
            pushOp(&opStack, currentToken.type);
        }

    }
}

static void binaryOp(){
    if(opStack.count == 0){
        pushOp(&opStack, currentToken.type);
    }else{
        uint8_t stackOp = peekOp(&opStack);
        if(stackOp != TOKEN_OPEN_PAREN && (precendence[stackOp] > precendence[currentToken.type])){
                run(&vm, popOp(&opStack));
        }else{
            pushOp(&opStack, currentToken.type);
        }
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
    [TOKEN_PLUS] = binaryOp,
    [TOKEN_MINUS] = binaryOp,
    [TOKEN_SLASH] = binaryOp,
    [TOKEN_STAR] = binaryOp,
    [TOKEN_MODULO] = binaryOp,
    [TOKEN_U_MINUS] = unaryOp,
    [TOKEN_OPEN_PAREN] = openParen,
    [TOKEN_CLOSE_PAREN] = closeParen,
    [TOKEN_INTEGER] = intNumber
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

