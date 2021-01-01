#include <stdio.h>
#include "scanner.h"

int main(){
    const char* buffer = "2 * 32 + 2";
    initScanner(buffer);
    Token token = scanToken();
    switch (token.type) {
        case TOKEN_INTEGER:
            printf("integer\n");
            printf("%d\n", token.value.number);
            break;
        default:
            printf("operator\n");
    
    }
    return 0;
}
