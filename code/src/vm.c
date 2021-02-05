#include <bits/stdint-uintn.h>
#include <stdio.h>
#include "data.h"
#include "defs.h"
#include "decl.h"

// TODO - resolve u_minus issue [serious]


typedef uint8_t (*FuncOp)();

typedef enum{
    INTERPRET_OK,
    INTERPRET_ERROR
} Status;

uint32_t* code;
ValueArray* valueStack;

static int localSlot1 = -1;
static int localSlot2 = -1;


static void debugVmCode(){
    int i = 0;
    printf("code - { ");
    while(*(code+i) != OP_EOF){
        switch(*(code + i)){
            case OP_READ_INT:
                i++;
                printf("%s from index_%d, ", "read_int", *(code+i));
                break;
            case OP_GLOBAL_GET:
                i++;
                printf("%s from index_%d, ", "global_get", *(code+i));
                break;
            case OP_GLOBAL_SET:
                i++;
                printf("%s from index_%d, ", "global_set", *(code+i));
                break;
            case OP_NIL:
                printf("%s, ", "undefined");
                break;
            case OP_LOCAL_GET:
                i++;
                printf("%s from slot_%d, ", "get_local", *(code+i));
                break;
            case OP_LOCAL_SET:
                printf("%s, ", "set_local");
                break;
            case OP_ASSIGN_LOCAL:
                i++;
                printf("%s from index_%d, ", "assign_local", *(code+i));
                break;
            case OP_ASSIGN_GLOBAL:
                i++;
                printf("%s from index_%d, ", "assign_global", *(code+i));
                break;
            case OP_UPDATE_LOCAL:
                i++;
                printf("%s from index_%d, ", "update_local", *(code+i));
                break;
            case OP_ASSIGN:
                i++;
                printf("%s from index_%d, ", "assinl", *(code+i));
                break;
            case OP_ADD:
                printf("%s, ", "add");
                break;
            case OP_LESS:
                printf("less, ");
                break;
            case OP_GREATER:
                printf("greater, ");
            case OP_U_MINUS:
                printf("%s, ", "u_minus");
                break;
            case OP_PRINT:
                printf("%s, ", "print");
                break;
            case OP_LEAVE:
                i++;
                printf("leave %d, ", *(code+i));
                break;
            case OP_PATCH_JUMP:
                i++;
                printf("jump to %d, ", *(code+i));
                break;
            default:
                printf("default, %d, ", *(code + i));
                

        }
        i++;
    }
    printf("eof");
    printf("}\n");
}


void initVm(VM *vm, char* source){
    
    compile(source);
    vm->vmCode = compiledChunk.vmCode.values;
    code = vm->vmCode;
    debugVmCode();
    initValueArray(&vm->valueStack, 10);
    valueStack = &vm->valueStack;
}

void freeVm(VM* vm){
    freeValueArray(&vm->valueStack);
    freeValueArray(&(compiledChunk.constants));
    freeIntArray(&(compiledChunk.vmCode));
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

static uint8_t nil(){
    code++;
    Value undefinedVar = {
        UNDEFINED,
        {
            .iNumber = 0
        }
    };
    pushValue(valueStack, undefinedVar);
    return INTERPRET_OK;
}

static uint8_t patch_jump(){
    code++;
    int jumpTo = *(code++);
    Value a = popValue(valueStack);
    if(falsy(&a)){
        code += jumpTo;
        return INTERPRET_OK;
    }
    /*printf("jump to - %d\n", *(code + jumpTo));*/
    return INTERPRET_OK;
}

static uint8_t jump(){
    code++;
    int jumpTo = *(code++);
    code += jumpTo;
    return INTERPRET_OK;
}

static uint8_t leave(){
    // free allocated memory in block
    code++;
    valueStack->count = *(code++);
    return INTERPRET_OK;
}

static uint8_t setGlobal(){
    code++;
    compiledChunk.constants.values[*(code++)] = popValue(valueStack);
    return INTERPRET_OK;
}



static uint8_t getGlobal(){
    code++;
    pushValue(valueStack, compiledChunk.constants.values[*code++]);
    return INTERPRET_OK;
}

static uint8_t op_true(){
    code++;
    Value value = {
        BOOL,
        {
            .iNumber = 1
        }
    };
    pushValue(valueStack, value);
    return INTERPRET_OK;
}
static uint8_t op_false(){
    code++;
    Value value = {
        BOOL,
        {
            .iNumber = 0
        }
    };
    pushValue(valueStack, value);
    return INTERPRET_OK;
}

static uint8_t readInt(){
    code++;
    pushValue(valueStack, compiledChunk.constants.values[*code++]);

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
    Value *a = peekValueArray(valueStack);
    if(!a->type){
        // number is integer
        a->as.iNumber = -(a->as.iNumber);
    }else if(a->type == FLOAT){
        // number is a float
        a->as.fNumber = -(a->as.fNumber);
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

static uint8_t getLocal(){
    code++;
    /*if(localSlot1 > -1){*/
        /*localSlot1 = *(code++);*/
    /*}else if(localSlot2 > -1){*/
        /*localSlot2 = *(code++);*/
    /*}*/
    pushValue(valueStack, valueStack->values[*(code++)]);
    return INTERPRET_OK;
}

static uint8_t updateLocal(){
    code++;
    valueStack->values[*(code++)] = popValue(valueStack);
    return INTERPRET_OK;
}

static uint8_t setLocal(){
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
    }else{
        printf("undefined\n");
    }
    code++;
    return INTERPRET_OK;
}

static uint8_t assignLocal(){
    code++;
    valueStack->values[*(code++)] = *peekValueArray(valueStack);
    return INTERPRET_OK;
}
static uint8_t assignGlobal(){
    code++;
    compiledChunk.constants.values[*(code++)] = *peekValueArray(valueStack);
    return INTERPRET_OK;
}

static uint8_t eof(){
    code++;
    return INTERPRET_ERROR;
}

FuncOp funcs[] = {
    [OP_ASSIGN_LOCAL] = assignLocal,
    [OP_ASSIGN_GLOBAL] = assignGlobal,
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
    [OP_GLOBAL_SET] = setGlobal,
    [OP_GLOBAL_GET] = getGlobal,
    [OP_NIL] = nil,
    [OP_LOCAL_SET] = setLocal,
    [OP_LOCAL_GET] = getLocal,
    [OP_UPDATE_LOCAL] = updateLocal,
    [OP_TRUE] = op_true,
    [OP_FALSE] = op_false,
    [OP_LEAVE] = leave,
    [OP_PATCH_JUMP] = patch_jump,
    [OP_JUMP] = jump,
    [OP_EOF] = eof


};


void interpret(VM *vm){
    
    while(funcs[*code](vm->valueStack) == INTERPRET_OK){
    }
}
