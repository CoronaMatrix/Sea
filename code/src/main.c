#include <stdio.h>
#include "scanner.h"
#include "utils/op_stack.h"

int main(){

    OpStack opStack;
    initOpStack(&opStack);

    pushOp(&opStack, TOKEN_INTEGER);
    pushOp(&opStack, TOKEN_PLUS);
    pushOp(&opStack, TOKEN_MINUS);
    pushOp(&opStack, TOKEN_SLASH);
    pushOp(&opStack, TOKEN_STAR);
    pushOp(&opStack, TOKEN_MODULO);
    int op;
    while((op = popOp(&opStack)) != 255){

        printf("%d\n", op);

    }

    freeOpStack(&opStack);

    return 0;
}
