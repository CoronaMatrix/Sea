#include <stdio.h>
#include <stdlib.h>
#include "utils/common.h"
#include "value.h"
#include "vm.h"
#include "table.h"
#include "object.h"
#define DEBUG_ON


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
    char *keyName3 = malloc(2);
    keyName3[0] = 'c';
    keyName3[1] = '\0';

    char *keyName4 = malloc(2);
    keyName4[0] = 'd';
    keyName4[1] = '\0';

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

    ObjString key2;
    key2.chars = keyName3;
    key2.length = 1;
    key2.hash = 123234434;
    key2.dist = GOLD_NUMBER;
    Obj obj2;
    obj2.objType = OBJ_STRING;
    key2.obj = obj2;

    Value value2;
    value2.type = FLOAT;
    value2.as.fNumber = 55.2;

    ObjString key3;
    key3.chars = keyName4;
    key3.length = 1;
    key3.hash = 4545454;
    key3.dist = GOLD_NUMBER;
    Obj obj3;
    obj3.objType = OBJ_STRING;
    key3.obj = obj3;

    Value value3;
    value3.type = STRING;
    value3.as.obj = (Obj*)(&key3);

    Table table;
    initTable(&table);
    tableSet(&table, &key, &value); 
    tableSet(&table, &key1, &value1); 
    tableSet(&table, &key2, &value2); 
    tableSet(&table, &key3, &value3); 
    /*tableDelete(&table, &key3);*/
    
    
    printf("%d\n", key.dist);
    /*printf("%d\n", key1.dist);*/

    /*Value returnValue1;*/
    /*Value returnValue2;*/

    /*tableGet(&table, &key, &returnValue1);*/
    /*tableGet(&table, &key1, &returnValue2);*/
    /*printf("%d, %d\n", returnValue1.as.iNumber, returnValue2.as.iNumber);*/
    
    /*Value returnValue;*/
    /*tableGet(&table, &key, &returnValue);*/

    /*if(tableDelete(&table, &key) == TRUE){*/
        /*printf("deleted\n");*/
    /*}*/
    /*if(tableGet(&table, &key, &returnValue) == FALSE){*/
        /*printf("not present\n");*/
    /*}*/
#ifdef DEBUG_ON
    debugTable(&table, 1);
#undef DEBUG_ON
#endif

    freeTable(&table);
    free(keyName1);
    free(keyName2);
    free(keyName3);
    free(keyName4);

    


    return 0;
}
