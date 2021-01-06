#include <stdio.h>
#include "utils/int_array.h"
#include "vm.h"
#include "compiler.h"
int main(){
    CompiledChunk compiledChunk = compile("1+1+1+1+1+1+1+1+1+1;\0");
    VM vm;
    vm.constants = compiledChunk.constants;
    vm.vmCode = compiledChunk.vmCode->values;

    initVm(&vm);

    /*while(*vm.vmCode != OP_EOF){*/
        /*if(*vm.vmCode == 21){*/
            /*vm.vmCode++;*/
            /*printf("%d\n", vm.constants->values[*vm.vmCode++].as.iNumber);*/
        /*}else{*/
            
        /*vm.vmCode++;*/
        /*}*/
        
    /*}*/

    interpret(&vm);
    freeVm(&vm);

    freeIntArray(compiledChunk.vmCode);

    return 0;
}
