#include <stdio.h>
#include <stdlib.h>
#include "utils/common.h"
#include "value.h"
#include "vm.h"
#include "table.h"
#include "object.h"
#define DEBUG_ON
 static char* readFile(const char* path){
      printf("%s path--\n", path);
      FILE* file = fopen(path, "rb");
  
      if(file == NULL){
          fprintf(stderr, "Could not open file \"%s\".\n", path);
          exit(74);
      }
  
      fseek(file, 0L, SEEK_END);
      size_t fileSize = ftell(file);
      rewind(file);
  
      char* buffer = (char*)malloc(fileSize + 1);
      if(buffer == NULL){
          fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
          exit(74);
      }
      size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
      if(bytesRead < fileSize){
          fprintf(stderr, "Could not read file \"%s\".\n",path); 
          exit(74);
     }
     buffer[bytesRead] = '\0';
 
     fclose(file);
     return buffer;
}


int main(int argc, const char* argv[]){

    VM vm;
    char *buffer = readFile(argv[1]);
    printf("%s\n", buffer);
    /*initVm(&vm, "let x = 1; let y = 2; let z = x + y; print z;\0");*/
    
    initVm(&vm, buffer);
    interpret(&vm);
#ifdef DEBUG_ON
    debugTable(&vm.globals, 1);
#endif
    freeVm(&vm);
    return 0;
}
