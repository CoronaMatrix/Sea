#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
#include "decl.h"
#include "data.h"


// TODO - identify unary minus inside expr not in scanner [serious]


static int assign_lg;
static int exprAssign = 0;
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

static OpPrec precendence[] = {
    [TOKEN_LESS] = LESS,
    [TOKEN_LESS_EQUAL] = LESS_EQUAL,
    [TOKEN_GREATER] = GREATER,
    [TOKEN_GREATER_EQUAL] = GREATER_EQUAL,
    [TOKEN_PLUS] = PLUS,
    [TOKEN_MINUS] = MINUS,
    [TOKEN_STAR] = MULTIPLY,
    [TOKEN_SLASH] = DIVIDE,
    [TOKEN_MODULO] = MODULO,
    [OP_ASSIGN_LOCAL] = ASSIGN,
    [OP_ASSIGN_GLOBAL] = ASSIGN,

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

// 0 for right and 1 for left
// TODO - correct the associativity
static int associativity[] = {

    [TOKEN_LESS] = 0,
    [TOKEN_LESS_EQUAL] = 0,
    [TOKEN_GREATER] = 0,
    [TOKEN_GREATER_EQUAL] = 0,
    [TOKEN_PLUS] = 1,
    [TOKEN_MINUS] = 1,
    [TOKEN_STAR] = 1,
    [TOKEN_SLASH] = 1,
    [TOKEN_MODULO] = 1,

    [TOKEN_EQUAL] = 0,
    [OP_ASSIGN_GLOBAL] = 0,
    [OP_ASSIGN_LOCAL] = 0,
    [TOKEN_EQUAL_EQUAL] = 0,
    [TOKEN_BANG] = 0,
    [TOKEN_BANG_EQUAL] = 0,
    [TOKEN_U_MINUS] = 0,
    [TOKEN_BITWISE_AND] = 0,
    [TOKEN_BITWISE_OR] = 0,
    [TOKEN_BITWISE_XOR] = 0,
    [TOKEN_BITWISE_NOT] = 0,
};


static void intNumber(){
    Value intValue = {
        INTEGER,
        {
            .iNumber = currentToken.value.number
        }
    };
   emit2(OP_READ_INT, pushValue(&(compiledChunk.constants), intValue), 0);
   scan_into();
}

static void booleanOp(){
    emit(match(TOKEN_TRUE) ? OP_TRUE : OP_FALSE, 0);
    scan_into();
}


static void arithmeticOp(){
    int tokenType = exprAssign > 0 ? assign_lg : currentToken.type;
    
   if(opStack.count == 0){
       pushIntArray(&opStack, tokenType);
    }else{
        // here we checking for precedence of op on stack >= precedence of current o
    
        while(!match(TOKEN_OPEN_PAREN) && (precendence[peekIntArray(&opStack)] + associativity[tokenType] > precendence[tokenType])){
            /*printf("runs\n");*/
            int op_code = popIntArray(&opStack);
            if(op_code == OP_ASSIGN_LOCAL || op_code == OP_ASSIGN_GLOBAL){
                emit2(op_code, popIntArray(&indexes), 0);
            }else{
                emit(op_code, 0);
            }
        }

        pushIntArray(&opStack, tokenType);
        
    }
   scan_into();
}



static void identifier(){
    
    int index = getSymbol(&symTable, scopeDepth > -1 ? -1 : 1,
            currentToken.value.string, currentToken.length);
    if(index < 0){
        char *name = malloc(currentToken.length + 1);
        memcpy(name, currentToken.value.string, currentToken.length);
        name[currentToken.length] = '\0';
        printf("Undeclared identifier: %s at %d\n", name, currentToken.line);
        free(name);
        exit(1);
    }
    scan_into();
    if(match(TOKEN_EQUAL)){
        assign_lg = symTable.isGlobal ? OP_ASSIGN_GLOBAL : OP_ASSIGN_LOCAL;
        pushIntArray(&indexes, index);
        exprAssign = 1;
        arithmeticOp();
        exprAssign = 0;
    }else{
        emit2(symTable.isGlobal ? OP_GLOBAL_GET : OP_LOCAL_GET, index, 0);
    }
}


static void openParen(){

    pushIntArray(&opStack, TOKEN_OPEN_PAREN);
    scan_into();

}
static void closeParen(){
    int op_code = peekIntArray(&opStack);
    while(peekIntArray(&opStack) != TOKEN_OPEN_PAREN && opStack.count != 0){
        if(op_code == OP_ASSIGN_LOCAL || op_code == OP_ASSIGN_GLOBAL){
            emit2(popIntArray(&opStack), popIntArray(&indexes), 0);
            
        }else{
            emit(popIntArray(&opStack), 0);
        }
    }
    popIntArray(&opStack);
    scan_into();
}

static ParseFun parse[] = {
    [TOKEN_EQUAL] = arithmeticOp,
    [OP_ASSIGN_GLOBAL] = arithmeticOp,
    [OP_ASSIGN_LOCAL] = arithmeticOp,
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
        if(parse[currentToken.type] == NULL){
            break;
        }
        parse[currentToken.type]();
        wrongm(TOKEN_EOF, "; expected but file ended");
    }
    while(opStack.count > 0){
        int op_code = popIntArray(&opStack);
        if(op_code == OP_ASSIGN_LOCAL || op_code == OP_ASSIGN_GLOBAL){
            emit2(op_code, popIntArray(&indexes), 0);
        }else{
            emit(op_code,0);
        }
    }
}

