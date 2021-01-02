#include <stdio.h>
#include "scanner.h"
#include "utils/op_stack.h"
#include "value.h"
#include "utils/value_stack.h"
#include "vm.h"

int main(){

    VM vm;
    OpStack opStack;
    ValueStack valueStack;
    initVm(&vm, &opStack, &valueStack);
    Value b;
    b.type = INTEGER;
    b.as.iNumber = 10;
    Value a;
    a.type = INTEGER;
    a.as.iNumber = 12;
    pushValue(&valueStack, a);
    pushValue(&valueStack, b);
    run(&vm, OP_ADD);
    run(&vm, OP_PRINT);
    freeVm(&vm);

    return 0;
}
