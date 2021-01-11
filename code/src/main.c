#include <stdio.h>
#include <stdlib.h>
#include "utils/int_array.h"
#include "utils/value_array.h"
#include "vm.h"
#include "compiler.h"

int main(){
    /*printf("%d\n", 90+32-(2*3)+9/3+(65*2-2)+199);*/
    /*CompiledChunk compiledChunk = compile("90+32-(2*3);\0");*/
    /*VM vm;*/
    /*vm.constants = compiledChunk.constants;*/
    /*vm.vmCode = compiledChunk.vmCode->values;*/

    /*initVm(&vm);*/

    /*[>while(*vm.vmCode != OP_EOF){<]*/
        /*[>if(*vm.vmCode == 21){<]*/
            /*[>vm.vmCode++;<]*/
            /*[>printf("%d\n", vm.constants->values[*vm.vmCode++].as.iNumber);<]*/
        /*[>}else{<]*/
            
        /*[>vm.vmCode++;<]*/
        /*[>}<]*/
        
    /*[>}<]*/



    /*interpret(&vm);*/
    /*freeVm(&vm);*/

    /*freeIntArray(compiledChunk.vmCode);*/
    /*printf("%d\n", ((IntArray*)compiledChunk.vmCode->values)->capacity);*/

    VM vm;
    initVm(&vm, "1+3;\0");

    interpret(&vm);


    freeVm(&vm);

    return 0;
}
