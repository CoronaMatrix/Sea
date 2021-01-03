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

int truth(Value* a, Value* b){
    if(!(a->type + b->type)){
        // both numbers are integers
       return a->as.iNumber == b->as.iNumber; 
    }
    // TODO: check for other types also
    // ex: int == double (convert int to double),
    // bool == int | double (convert int | double to bool)
    return 0;
}

static void left_shift(ValueStack* valueStack){

    Value b = popValue(valueStack);
    Value a = popValue(valueStack);

    if(!(a.type + b.type)){
        // both are integers
        Value value;
        value.type = INTEGER;
        value.as.iNumber = a.as.iNumber << b.as.iNumber;
        pushValue(valueStack, value);
    }else{
        printf("RE: Operands must be of type integers\n");
    }
}

static void right_shift(ValueStack* valueStack){

    Value b = popValue(valueStack);
    Value a = popValue(valueStack);

    if(!(a.type + b.type)){
        // both are integers
        Value value;
        value.type = INTEGER;
        value.as.iNumber = a.as.iNumber >> b.as.iNumber;
        pushValue(valueStack, value);
    }else{
        printf("RE: Operands must be of type integers\n");
    }
}

static void bitwise_or(ValueStack* valueStack){
    
    Value b = popValue(valueStack);
    Value a = popValue(valueStack);

    if(!(a.type + b.type)){
        // both are integers
        Value value;
        value.type = INTEGER;
        value.as.iNumber = a.as.iNumber | b.as.iNumber;
        pushValue(valueStack, value);
    }else{
        printf("RE: Operands must be of type integers\n");
    }
}

static void bitwise_xor(ValueStack* valueStack){

    Value b = popValue(valueStack);
    Value a = popValue(valueStack);

    if(!(a.type + b.type)){
        // both are integers
        Value value;
        value.type = INTEGER;
        value.as.iNumber = a.as.iNumber ^ b.as.iNumber;
        pushValue(valueStack, value);
    }else{
        printf("RE: Operands must be of type integers\n");
    }
}

static void bitwise_and(ValueStack* valueStack){

    Value b = popValue(valueStack);
    Value a = popValue(valueStack);

    if(!(a.type + b.type)){
        // both are integers
        Value value;
        value.type = INTEGER;
        value.as.iNumber = a.as.iNumber & b.as.iNumber;
        pushValue(valueStack, value);
    }else{
        printf("RE: Operands must be of type integers\n");
    }
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
        printf("min: %d\n", value.as.iNumber);
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

static void u_not(ValueStack* valueStack){

    Value a = popValue(valueStack);
    if(a.type == BOOL){
        // TODO: support integers and doubles and strings
        // number is BOOL 
        Value value;
        value.type = BOOL;
        // TODO: create a separate function for falsyness
        value.as.iNumber = a.as.iNumber == 0 ? 1 : 0;
        pushValue(valueStack, value);
    }else{
        printf("RE: Operand must be of type integer\n");
    }

}


static void multiply(ValueStack* valueStack){

    Value b = popValue(valueStack);
    Value a = popValue(valueStack);
    if(!(a.type + b.type)){
        // both are integers
        
        Value value;
        value.type = INTEGER;
        value.as.iNumber = a.as.iNumber * b.as.iNumber;
        printf("mul: %d\n", value.as.iNumber);
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
        printf("div: %d\n", value.as.iNumber);
        pushValue(valueStack, value);
    }else{
        printf("RE: Operands must be of type integers\n");
    }

}

static void modulo(ValueStack* valueStack){
    Value b = popValue(valueStack);
    Value a = popValue(valueStack);

    if(!(a.type + b.type)){
        // both are integers {note: modulo does not support doubles}
        Value value;
        value.type = INTEGER;
        value.as.iNumber = a.as.iNumber % b.as.iNumber;
        printf("mod: %d\n", value.as.iNumber);
        pushValue(valueStack, value);
    }else{
        printf("RE: Operands must be of type integers\n");
    }
}

static void less(ValueStack* valueStack){
    
    Value b = popValue(valueStack);
    Value a = popValue(valueStack);

    if(!(a.type + b.type)){
        // both are integers {note: relational ops requires operands to be numbers int or double}
        Value value;
        value.type = BOOL;
        value.as.iNumber = a.as.iNumber < b.as.iNumber;
        pushValue(valueStack, value);
    }else{
        printf("RE: Operands must be of type numbers\n");
    }

}

static void greater(ValueStack* valueStack){

    Value b = popValue(valueStack);
    Value a = popValue(valueStack);

    if(!(a.type + b.type)){
        // both are integers {note: relational ops requires operands to be numbers int or double}
        Value value;
        value.type = BOOL;
        value.as.iNumber = a.as.iNumber > b.as.iNumber;
        pushValue(valueStack, value);
    }else{
        printf("RE: Operands must be of type numbers\n");
    }
}

static void less_equal(ValueStack* valueStack){

    Value b = popValue(valueStack);
    Value a = popValue(valueStack);

    if(!(a.type + b.type)){
        // both are integers {note: relational ops requires operands to be numbers int or double}
        Value value;
        value.type = BOOL;
        value.as.iNumber = !(a.as.iNumber > b.as.iNumber);
        pushValue(valueStack, value);
    }else{
        printf("RE: Operands must be of type numbers\n");
    }
}

static void greater_equal(ValueStack* valueStack){

    Value b = popValue(valueStack);
    Value a = popValue(valueStack);

    if(!(a.type + b.type)){
        // both are integers {note: relational ops requires operands to be numbers int or double}
        Value value;
        value.type = BOOL;
        value.as.iNumber = !(a.as.iNumber < b.as.iNumber);
        pushValue(valueStack, value);
    }else{
        printf("RE: Operands must be of type numbers\n");
    }
}


static void is_equal(ValueStack* valueStack){
    Value b = popValue(valueStack);
    Value a = popValue(valueStack);

    Value value;
    value.type = BOOL;
    value.as.iNumber = truth(&a, &b);
    pushValue(valueStack, value);
}

static void is_not_equal(ValueStack* valueStack){

    Value b = popValue(valueStack);
    Value a = popValue(valueStack);

    Value value;
    value.type = BOOL;
    value.as.iNumber = !(truth(&a, &b));
    pushValue(valueStack, value);
}

static void print(ValueStack* valueStack){
    printf("%d\n", popValue(valueStack).as.iNumber);
}

static void assign(ValueStack* valueStack){}

/*FuncOp funcs[] = {*/
  /*assign, bitwise_or, bitwise_xor, bitwise_and, is_equal, is_not_equal, less, greater, less_equal, greater_equal,  add, minus, divide, multiply, modulo, u_minus, u_not, print*/
/*};*/

FuncOp funcs[] = {
    [OP_ASSIGN] = assign,
    [OP_BITWISE_OR] = bitwise_or,
    [OP_BITWISE_XOR] = bitwise_xor,
    [OP_BITWISE_AND] = bitwise_and,
    [OP_ADD]  = add,
    [OP_MINUS] = minus,
    [OP_MULTIPLY] = multiply,
    [OP_DIVIDE] = divide,
    [OP_MODULO] = modulo,
    [OP_U_MINUS] = u_minus,
    [OP_NOT] = u_not,
    [OP_PRINT] = print,
    [OP_LESS] = less,
    [OP_GREAER_EQUAL] = greater_equal,
    [OP_GREATER] = greater,
    [OP_LESS_EQUAL] = less_equal,
    [OP_EQUAL] = is_equal,
    [OP_NOT_EQUAL] = is_not_equal,
    [OP_LEFT_SHIFT] = left_shift,
    [OP_RIGHT_SHIFT] = right_shift,
};

void run(VM *vm, OpCode code){
    funcs[code](vm->valueStack);
}

