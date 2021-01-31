#include <stdio.h>
#include <stdlib.h>
#include "utils/common.h"
#include "value.h"
#include "vm.h"
#include "table.h"
#include "object.h"
#define DEBUG_ON


int main(){

    VM vm;
    initVm(&vm, "let x = 1; let y = 2; let z = x + y; print z;\0");
    interpret(&vm);
#ifdef DEBUG_ON
    debugTable(&vm.globals, 1);
#endif
    freeVm(&vm);
    return 0;
}
