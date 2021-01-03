#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "scanner.h"

const char* source;
int lineNumber;
int previousToken = -1;

static int isDigit(char c){
    return c > '/' && c < ':';
}

static int escape(){
    char c = *source;
        
    return c == ' ';
}

void initScanner(const char *buffer){

    source = buffer;
    lineNumber = 1;
}

static Token makeToken(TokenType type, int line, TokenValue value){
    Token token = {
        type, line, value
    };
    source++;
    return token;
}

static Token scanNumber(){

    int overflow = 0;
    int result = 0;

    char c = *source;
    while(isDigit(c) > 0){
        if(result > INT_MAX / 10) overflow = 1;
        else if(result * 10 > (INT_MAX - c + '0')) overflow = 1;
        else result = result * 10 + c - '0';
        c = *(++source);
    }

    TokenValue value;
    if(overflow) value.number = INT_MAX;
    else value.number = result;
    Token token = {
        TOKEN_INTEGER, lineNumber, value
    };

    previousToken = TOKEN_INTEGER; 

    return token;
}


Token scanToken(){

    while(escape()){
        source++;
    }

    TokenValue value;
    switch(*source){

        case '+':
            previousToken = TOKEN_PLUS;
            return makeToken(TOKEN_PLUS, lineNumber, value);

        case '-':
            if(previousToken == TOKEN_OPEN_PAREN || previousToken == -1){
                previousToken = TOKEN_U_MINUS;
                return makeToken(TOKEN_U_MINUS, lineNumber, value);
            }
            previousToken = TOKEN_MINUS;
            return makeToken(TOKEN_MINUS, lineNumber, value);

        case '*':
            previousToken = TOKEN_STAR;
            return makeToken(TOKEN_STAR, lineNumber, value);

        case '/':
            previousToken = TOKEN_SLASH;
            return makeToken(TOKEN_SLASH, lineNumber, value);

        case '%':
            previousToken = TOKEN_MODULO;
            return makeToken(TOKEN_MODULO, lineNumber, value);

        case '(':
            previousToken = TOKEN_OPEN_PAREN;
            return makeToken(TOKEN_OPEN_PAREN, lineNumber, value);

        case ')':
            previousToken = TOKEN_CLOSE_PAREN;
            return makeToken(TOKEN_CLOSE_PAREN, lineNumber, value);

        case ';':
            previousToken = TOKEN_SEMICOLON;
            return makeToken(TOKEN_SEMICOLON, lineNumber, value);

        default:
            return scanNumber();

    }

}
