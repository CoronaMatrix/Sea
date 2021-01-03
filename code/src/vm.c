#include <stdio.h>
#include "value.h"
#include "vm.h"
#include "utils/op_stack.h"
#include "utils/value_stack.h"

typedef void (*FuncOp)(ValueStack* valueStack);

void initVm(VM *vm, OpStack *opStack, ValueStack *valueStack){
    initOpStack(opStack);
    initValueStack(valueStack);
    vm->opStack = opStack;
    vm->valueStack = valueStack;
}

void freeVm(VM* vm){
    freeOpStack(vm->opStack);
    freeValueStack(vm->valueStack);
}

static void add(ValueStack* valueStack){

    Value b = popValue(valueStack);
    Value a = popValue(valueStack);

    if(!(a.type + b.type)){
        // both are integers
        Value value;
        value.type = INTEGER;
        value.as.iNumber = a.as.iNumber + b.as.iNumber;
        pushValue(valueStack, value);
    }else{
        printf("RE: Operands must be of type integers\n");
    }

}

static void minus(ValueStack* valueStack){

    Value b = popValue(valueStack);
    Value a = popValue(valueStack);
    if(!(a.type + b.type)){
        // both are integers
        Value value;
        value.type = INTEGER;
        value.as.iNumber = a.as.iNumber - b.as.iNumber;
        pushValue(valueStack, value);
    }else{
        printf("RE: Operands must be of type integers\n");
    }

}
static void u_minus(ValueStack* valueStack){
    Value a = popValue(valueStack);
    if(!a.type){
        // number is integer
        Value value;
        value.type = INTEGER;
        value.as.iNumber = -(a.as.iNumber);
        pushValue(valueStack, value);
    }else{
        printf("RE: Operand must be of type integer\n");
    }
}
static void multiply(ValueStack* valueStack){

    Value b = popValue(valueStack);
    Value a = popValue(valueStack);
    printf("mult %d %d\n", a.as.iNumber, b.as.iNumber);
    if(!(a.type + b.type)){
        // both are integers
        Value value;
        value.type = INTEGER;
        value.as.iNumber = a.as.iNumber * b.as.iNumber;
        pushValue(valueStack, value);
    }else{
        printf("RE: Operands must be of type integers\n");
    }

}
static void divide(ValueStack* valueStack){

    Value b = popValue(valueStack);
    Value a = popValue(valueStack);
    if(!(a.type + b.type)){
        // both are integers
        Value value;
        value.type = INTEGER;
        value.as.iNumber = a.as.iNumber / b.as.iNumber;
        pushValue(valueStack, value);
    }else{
        printf("RE: Operands must be of type integers\n");
    }

}

static void print(ValueStack* valueStack){
    printf("%d\n", popValue(valueStack).as.iNumber);
}

FuncOp funcs[] = {
    add, minus, divide, multiply, NULL,u_minus, print
};

void run(VM *vm, OpCode code){
    funcs[code](vm->valueStack);
}

