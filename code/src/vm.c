#include <bits/stdint-uintn.h>
#include <stdio.h>
#include "value.h"
#include "vm.h"
#include "compiler.h"
#include "utils/int_array.h"
#include "utils/value_array.h"

typedef uint8_t (*FuncOp)();

typedef enum{
    INTERPRET_OK,
    INTERPRET_ERROR
} Status;

uint32_t* code;
ValueArray* valueStack;
CompiledChunk compiledChunk;

void initVm(VM *vm, const char* source){
    
    compiledChunk = compile(source);
    vm->vmCode = compiledChunk.vmCode->values;
    code = vm->vmCode;
    initValueArray(&vm->valueStack, 20);
    valueStack = &vm->valueStack;
}

void freeVm(VM* vm){
    freeValueArray(&vm->valueStack);
    freeValueArray(compiledChunk.constants);
    freeIntArray(compiledChunk.vmCode);
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

static uint8_t readInt(){
    printf("read int\n");
    code++;
    pushValue(valueStack, compiledChunk.constants->values[*code++]);

    return INTERPRET_OK;
}

static uint8_t left_shift(){

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
        return INTERPRET_ERROR;
    }
    code++;
    return INTERPRET_OK;
}

static uint8_t right_shift(){

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
        return INTERPRET_ERROR;
    }
    code++;
    return INTERPRET_OK;
}

static uint8_t bitwise_not(){
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
        return INTERPRET_ERROR;
    }
    code++;
    return INTERPRET_OK;
}

static uint8_t bitwise_or(){
    
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
        return INTERPRET_ERROR;
    }
    code++;
    return INTERPRET_OK;
}

static uint8_t bitwise_xor(){

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
        return INTERPRET_ERROR;
    }
    code++;
    return INTERPRET_OK;
}

static uint8_t bitwise_and(){

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
        return INTERPRET_ERROR;
    }
    code++;
    return INTERPRET_OK;
}

static uint8_t add(){

    Value b = popValue(valueStack);
    Value a = popValue(valueStack);
    printf("size: %d\n", valueStack->count);
    
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
        code++;
        return INTERPRET_ERROR;
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
    code++;
    return INTERPRET_OK;

}

static uint8_t minus(){
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
        return INTERPRET_ERROR;
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
    code++;
    return INTERPRET_OK;


}
static uint8_t u_minus(){
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
        return INTERPRET_ERROR;
    }
    code++;
    return INTERPRET_OK;
}

static uint8_t u_not(){
    Value a = popValue(valueStack);
    Value value = {
        BOOL,
        {
            .iNumber = falsy(&a)
        }
    };
    pushValue(valueStack, value);
    code++;
    return INTERPRET_OK;
}


static uint8_t multiply(){

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
        return INTERPRET_ERROR;
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
    code++;
    return INTERPRET_OK;

}
static uint8_t divide(){

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
        return INTERPRET_ERROR;
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
    code++;
    return INTERPRET_OK;

}

static uint8_t modulo(){
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
        return INTERPRET_ERROR;
    }
    code++;
    return INTERPRET_OK;
}

static uint8_t less(){
    
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
        return INTERPRET_ERROR;
    }
    code++;
    return INTERPRET_OK;

}

static uint8_t greater(){

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
        return INTERPRET_ERROR;
    }
    code++;
    return INTERPRET_OK;
}

static uint8_t less_equal(){
    

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
        return INTERPRET_ERROR;
    }
    code++;
    return INTERPRET_OK;
}

static uint8_t greater_equal(){

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
        return INTERPRET_ERROR;
    }
    code++;
    return INTERPRET_OK;
}


static uint8_t is_equal(){
    Value b = popValue(valueStack);
    Value a = popValue(valueStack);

    Value value = {
        BOOL,
        {
            .iNumber = truth(&a, &b)
        }
    };
    pushValue(valueStack, value);
    code++;
    return INTERPRET_OK;
}

static uint8_t is_not_equal(){

    Value b = popValue(valueStack);
    Value a = popValue(valueStack);

    Value value = {
        BOOL,
        {
            .iNumber = !(truth(&a, &b))
        }
    };
    pushValue(valueStack, value);
    code++;
    return INTERPRET_OK;
}

static uint8_t print(){
    Value value = popValue(valueStack);



    if(!value.type){
        printf("%d\n", value.as.iNumber);
    }else if(value.type == 1){
        printf("%s\n", value.as.iNumber == 0 ? "false" : "true");
    }else if(value.type == 2){
        printf("%f\n", value.as.fNumber);
    }
    code++;
    return INTERPRET_OK;
}

static uint8_t assign(){
    code++;
    return INTERPRET_OK;
}

static uint8_t eof(){
    code++;
    return INTERPRET_ERROR;
}

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
    [OP_READ_INT] = readInt,
    [OP_EOF] = eof
};

/*void run(){*/
    /*funcs[code](vm->valueStack);*/
/*}*/

void interpret(VM *vm){
    
    while(funcs[*code](vm->valueStack) == INTERPRET_OK);
}
