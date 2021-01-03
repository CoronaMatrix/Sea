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
    uint8_t vType = a->type + b->type;
    if(!vType || vType == 2){
        // both numbers are integers or bools
       return a->as.iNumber == b->as.iNumber; 
    }else if(vType == 4){
        // both numbers are floats
        return a->as.fNumber == b->as.fNumber;
    }else if(a->type == BOOL){
        if(b->type == FLOAT) return a->as.iNumber == !!b->as.fNumber;
        return !!a->as.iNumber == !!b->as.iNumber;
        
    }else if(b->type == BOOL){
        if(a->type == FLOAT) return !!a->as.fNumber == b->as.iNumber;
        return !!a->as.iNumber == !!b->as.iNumber;
    }else if(!a->type){
        return (float)a->as.iNumber == b->as.fNumber;
    }else if(!b->type){
        return a->as.fNumber == (float)b->as.iNumber;
    }
    // TODO: check for other types also
    // ex: int == double (convert int to double),
    // bool == int | double (convert int | double to bool)
    return 0;
}

int falsy(Value* a){
    if(a->type < 2){
        // bool and integer
        return !a->as.iNumber;
    }else if(a->type == 2){
        // float
        return !a->as.fNumber;
    }
    // TODO: handle strings
    return 0;
}
     



static void left_shift(ValueStack* valueStack){

    Value b = popValue(valueStack);
    Value a = popValue(valueStack);

    if(!(a.type + b.type)){
        // both are integers
        Value value = {
            INTEGER,
            {
                .iNumber = a.as.iNumber << b.as.iNumber
            }
        };
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
        Value value = {
            INTEGER,
            {
                .iNumber = a.as.iNumber >> b.as.iNumber
            }
        };
        pushValue(valueStack, value);
    }else{
        printf("RE: Operands must be of type integers\n");
    }
}

static void bitwise_not(ValueStack* valueStack){
    Value a = popValue(valueStack);
    if(!a.type){
        // Operand is integer
        Value value = {
            INTEGER,
            {
                .iNumber = ~a.as.iNumber
            }
        };

        pushValue(valueStack, value);

    }else{
        printf("RE: Operand must be of type integer\n");
    }
}

static void bitwise_or(ValueStack* valueStack){
    
    Value b = popValue(valueStack);
    Value a = popValue(valueStack);

    if(!(a.type + b.type)){
        // both are integers
        Value value = {
            INTEGER,
            {
                .iNumber = a.as.iNumber | b.as.iNumber
            }
        };
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
        Value value = {
            INTEGER,
            {
                .iNumber = a.as.iNumber ^ b.as.iNumber
            }
        };
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
        Value value = {
            INTEGER,
            {
                .iNumber = a.as.iNumber & b.as.iNumber
            }
        };
        pushValue(valueStack, value);
    }else{
        printf("RE: Operands must be of type integers\n");
    }
}

static void add(ValueStack* valueStack){

    Value b = popValue(valueStack);
    Value a = popValue(valueStack);

    uint8_t vType = a.type + b.type;

    if(!vType){
        // both are integers
        Value value = {
            INTEGER,
            {
                .iNumber = a.as.iNumber + b.as.iNumber
            }
        };
        pushValue(valueStack, value);
    }else if(vType == 4){
        // both are floats
        Value value = {
            FLOAT,
            {
                .fNumber = a.as.fNumber + b.as.fNumber
            }
        };
        pushValue(valueStack, value);
    }else if(a.type == BOOL || b.type == BOOL){
        // both are bools
        printf("RE+: operands must be of type numbers\n");
    }else if(!a.type){
        Value value = {
            INTEGER,
            {
                .fNumber = a.as.iNumber + b.as.fNumber
            }
        };
        pushValue(valueStack, value);
    }else if(!b.type){
        Value value = {
            INTEGER,
            {
                .fNumber = a.as.fNumber + b.as.iNumber
            }
        };
        pushValue(valueStack, value);
    }

}

static void minus(ValueStack* valueStack){
    Value b = popValue(valueStack);
    Value a = popValue(valueStack);

    uint8_t vType = a.type + b.type;

    if(!vType){
        // both are integers
        Value value = {
            INTEGER,
            {
                .iNumber = a.as.iNumber - b.as.iNumber
            }
        };
        pushValue(valueStack, value);
    }else if(vType == 4){
        // both are floats
        Value value = {
            FLOAT,
            {
                .fNumber = a.as.fNumber - b.as.fNumber
            }
        };
        pushValue(valueStack, value);
    }else if(a.type == BOOL || b.type == BOOL){
        // both are bools
        printf("RE: operands must be of type numbers\n");
    }else if(!a.type){
        Value value = {
            INTEGER,
            {
                .fNumber = a.as.iNumber - b.as.fNumber
            }
        };
        pushValue(valueStack, value);
    }else if(!b.type){
        Value value = {
            INTEGER,
            {
                .fNumber = a.as.fNumber - b.as.iNumber
            }
        };
        pushValue(valueStack, value);
    }


}
static void u_minus(ValueStack* valueStack){
    Value a = popValue(valueStack);
    if(!a.type){
        // number is integer
        Value value = {
            INTEGER,
            {
                .iNumber = -(a.as.iNumber)
            }
        };
        pushValue(valueStack, value);
    }else if(a.type == FLOAT){
        // number is a float
        Value value = {
            FLOAT,
            {
                .fNumber = -(a.as.fNumber)
            }
        };
        pushValue(valueStack, value);
    }else{
        printf("RE: Operand must be of type integer\n");
    }
}

static void u_not(ValueStack* valueStack){
    Value a = popValue(valueStack);
    Value value = {
        BOOL,
        {
            .iNumber = falsy(&a)
        }
    };
    pushValue(valueStack, value);
}




static void multiply(ValueStack* valueStack){

    Value b = popValue(valueStack);
    Value a = popValue(valueStack);

    uint8_t vType = a.type + b.type;

    if(!vType){
        // both are integers
        Value value = {
            INTEGER,
            {
                .iNumber = a.as.iNumber * b.as.iNumber
            }
        };
        pushValue(valueStack, value);
    }else if(vType == 4){
        // both are floats
        Value value = {
            FLOAT,
            {
                .fNumber = a.as.fNumber * b.as.fNumber
            }
        };
        pushValue(valueStack, value);
    }else if(a.type == BOOL || b.type == BOOL){
        // both are bools
        printf("RE: operands must be of type numbers\n");
    }else if(!a.type){
        Value value = {
            INTEGER,
            {
                .fNumber = a.as.iNumber * b.as.fNumber
            }
        };
        pushValue(valueStack, value);
    }else if(!b.type){
        Value value = {
            INTEGER,
            {
                .fNumber = a.as.fNumber * b.as.iNumber
            }
        };
        pushValue(valueStack, value);
    }

}
static void divide(ValueStack* valueStack){

    Value b = popValue(valueStack);
    Value a = popValue(valueStack);

    uint8_t vType = a.type + b.type;

    if(!vType){
        // TODO: When dividing two integers produces a floating point then convert any one to the float and save it into float like 4/3 == 1.-- not 1
        // both are integers

        Value value = {
            INTEGER,
            {
                .iNumber = a.as.iNumber / b.as.iNumber
            }
        };
        pushValue(valueStack, value);
    }else if(vType == 4){
        // both are floats
        Value value = {
            FLOAT,
            {
                .fNumber = a.as.fNumber / b.as.fNumber
            }
        };
        pushValue(valueStack, value);
    }else if(a.type == BOOL || b.type == BOOL){
        // both are bools
        printf("RE: operands must be of type numbers\n");
    }else if(!a.type){
        Value value = {
            INTEGER,
            {
                .fNumber = a.as.iNumber / b.as.fNumber
            }
        };
        pushValue(valueStack, value);
    }else if(!b.type){
        Value value = {
            INTEGER,
            {
                .fNumber = a.as.fNumber * b.as.iNumber
            }
        };
        pushValue(valueStack, value);
    }

}

static void modulo(ValueStack* valueStack){
    Value b = popValue(valueStack);
    Value a = popValue(valueStack);

    if(!(a.type + b.type)){
        // both are integers {note: modulo does not support doubles}
        Value value = {
            INTEGER,
            {
                .iNumber = a.as.iNumber % b.as.iNumber
            }
        };
        pushValue(valueStack, value);
    }else{
        printf("RE: Operands must be of type integers\n");
    }
}

static void less(ValueStack* valueStack){
    
    Value b = popValue(valueStack);
    Value a = popValue(valueStack);

    uint8_t vType = a.type + b.type;

    if(vType < 3){
        // both are integers or bools
        Value value = {
            BOOL,
            {
                .iNumber = a.as.iNumber < b.as.iNumber
            }
        };
        pushValue(valueStack, value);
    }else if(vType == 4){
        // both are floats
        Value value = {
            BOOL,
            {
                .iNumber = a.as.fNumber < b.as.fNumber
            }
        };
        pushValue(valueStack, value);
    }else if(!a.type){
        Value value = {
            BOOL,
            {
                .iNumber = a.as.iNumber < b.as.fNumber
            }
        };
        pushValue(valueStack, value);
    }else if(!b.type){
        Value value = {
            BOOL,
            {
                .iNumber = a.as.fNumber < b.as.iNumber
            }
        };
        pushValue(valueStack, value);
    }else{
        printf("RE: operands must be of type numbers or integers\n");
    }

}

static void greater(ValueStack* valueStack){

    Value b = popValue(valueStack);
    Value a = popValue(valueStack);

    uint8_t vType = a.type + b.type;

    if(vType < 3){
        // both are integers or bools
        Value value = {
            BOOL,
            {
                .iNumber = a.as.iNumber > b.as.iNumber
            }
        };
        pushValue(valueStack, value);
    }else if(vType == 4){
        // both are floats
        Value value = {
            BOOL,
            {
                .iNumber = a.as.fNumber > b.as.fNumber
            }
        };
        pushValue(valueStack, value);
    }else if(!a.type){
        Value value = {
            BOOL,
            {
                .iNumber = a.as.iNumber > b.as.fNumber
            }
        };
        pushValue(valueStack, value);
    }else if(!b.type){
        Value value = {
            BOOL,
            {
                .iNumber = a.as.fNumber > b.as.iNumber
            }
        };
        pushValue(valueStack, value);
    }else{
        printf("RE: operands must be of type numbers or bools\n");
    }
}

static void less_equal(ValueStack* valueStack){
    

    Value b = popValue(valueStack);
    Value a = popValue(valueStack);

    uint8_t vType = a.type + b.type;

    if(vType < 3){
        // both are integers
        Value value = {
            BOOL,
            {
                .iNumber = !(a.as.iNumber > b.as.iNumber)
            }
        };
        pushValue(valueStack, value);
    }else if(vType == 4){
        // both are floats
        Value value = {
            BOOL,
            {
                .iNumber = !(a.as.iNumber > b.as.iNumber)
            }
        };
        pushValue(valueStack, value);
    }else if(!a.type){
        Value value = {
            BOOL,
            {
                .iNumber = !(a.as.iNumber > b.as.iNumber)
            }
        };
        pushValue(valueStack, value);
    }else if(!b.type){
        Value value = {
            BOOL,
            {
                .iNumber = !(a.as.iNumber > b.as.iNumber)
            }
        };
        pushValue(valueStack, value);
    }else{
        printf("RE: operands must be of type numbers or bools\n");
    }
}

static void greater_equal(ValueStack* valueStack){

    Value b = popValue(valueStack);
    Value a = popValue(valueStack);

    uint8_t vType = a.type + b.type;

    if(vType < 3){
        // both are integers
        Value value = {
            BOOL,
            {
                .iNumber = !(a.as.iNumber < b.as.iNumber)
            }
        };
        pushValue(valueStack, value);
    }else if(vType == 4){
        // both are floats
        Value value = {
            BOOL,
            {
                .iNumber = !(a.as.iNumber < b.as.iNumber)
            }
        };
        pushValue(valueStack, value);
    }else if(!a.type){
        Value value = {
            BOOL,
            {
                .iNumber = !(a.as.iNumber < b.as.iNumber)
            }
        };
        pushValue(valueStack, value);
    }else if(!b.type){
        Value value = {
            BOOL,
            {
                .iNumber = !(a.as.iNumber < b.as.iNumber)
            }
        };
        pushValue(valueStack, value);
    }else {
        printf("RE: operands must be of type numbers\n");
    }
}


static void is_equal(ValueStack* valueStack){
    Value b = popValue(valueStack);
    Value a = popValue(valueStack);

    Value value = {
        BOOL,
        {
            .iNumber = truth(&a, &b)
        }
    };
    pushValue(valueStack, value);
}

static void is_not_equal(ValueStack* valueStack){

    Value b = popValue(valueStack);
    Value a = popValue(valueStack);

    Value value = {
        BOOL,
        {
            .iNumber = !(truth(&a, &b))
        }
    };
    pushValue(valueStack, value);
}

static void print(ValueStack* valueStack){
    Value value = popValue(valueStack);

    if(!value.type){
        printf("%d\n", value.as.iNumber);
    }else if(value.type == 1){
        printf("%s\n", value.as.iNumber == 0 ? "false" : "true");
    }else if(value.type == 2){
        printf("%f\n", value.as.fNumber);
    }
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
    [OP_BITWISE_NOT] = bitwise_not,
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

