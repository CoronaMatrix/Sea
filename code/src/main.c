#include <stdio.h>
#include <stdlib.h>
#include "utils/common.h"
#include "value.h"
#include "vm.h"
#include "table.h"
#include "object.h"


int main(){

    /*VM vm;*/
    /*initVm(&vm, "1+3;\0");*/
    /*interpret(&vm);*/
    /*freeVm(&vm);*/
    
    char *keyName1 = malloc(2);
    keyName1[0] = 'a';
    keyName1[1] = '\0';

    char *keyName2 = malloc(2);
    keyName2[0] = 'b';
    keyName2[1] = '\0';

    ObjString key;
    key.chars = keyName1;
    key.length = 1;
    key.hash = 123234234;
    key.dist = GOLD_NUMBER;
    Obj obj;
    obj.objType = OBJ_STRING;
    key.obj = obj;

    Value value;
    value.type = INTEGER;
    value.as.iNumber = 55;

    ObjString key1;
    key1.chars = keyName2;
    key1.length = 1;
    key1.hash = 23424241;
    key1.dist = GOLD_NUMBER;
    Obj obj1;
    obj1.objType = OBJ_STRING;
    key1.obj = obj1;

    Value value1;
    value1.type = INTEGER;
    value1.as.iNumber = 65;

    Table table;
    initTable(&table);
    tableSet(&table, &key, &value); 
    tableSet(&table, &key1, &value1); 
    /*printf("%d\n", key.dist);*/
    /*printf("%d\n", key1.dist);*/

    /*Value returnValue1;*/
    /*Value returnValue2;*/

    /*tableGet(&table, &key, &returnValue1);*/
    /*tableGet(&table, &key1, &returnValue2);*/
    /*printf("%d\n%d\n", returnValue1.as.iNumber, returnValue2.as.iNumber);*/
    
    /*Value returnValue;*/
    /*tableGet(&table, &key, &returnValue);*/

    /*if(tableDelete(&table, &key) == TRUE){*/
        /*printf("deleted\n");*/
    /*}*/
    /*if(tableGet(&table, &key, &returnValue) == FALSE){*/
        /*printf("not present\n");*/
    /*}*/
    debugTable(&table);

    freeTable(&table);
    free(keyName1);
    free(keyName2);

    


    return 0;
}
