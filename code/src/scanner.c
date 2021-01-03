#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "scanner.h"

const char* source;
int lineNumber;
int previousToken = -1; // it need to be reset for every other expression

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
            printf("scanTokenMinus\n");
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

        case '^':
            previousToken = TOKEN_BITWISE_XOR;
            return makeToken(TOKEN_BITWISE_XOR, lineNumber, value);

        case '>':
            // check for = character | check for > 
            if(*(source + 1) == '='){
                previousToken = TOKEN_GREATER_EQUAL;
                source++;
                return makeToken(TOKEN_GREATER_EQUAL, lineNumber, value);
            }else if(*(source + 1) == '>'){
                // right shift token
                previousToken = TOKEN_RIGHT_SHIFT;
                source++;
                return makeToken(TOKEN_RIGHT_SHIFT, lineNumber, value);
            }
            previousToken = TOKEN_GREATER;
            return makeToken(TOKEN_GREATER, lineNumber, value);

        case '<':

            // check for = character | check for < 
            if(*(source + 1) == '='){
                previousToken = TOKEN_LESS_EQUAL;
                source++;
                return makeToken(TOKEN_LESS_EQUAL, lineNumber, value);
            }else if(*(source + 1) == '<'){
                // left shift token
                previousToken = TOKEN_LEFT_SHIFT;
                source++;
                return makeToken(TOKEN_LEFT_SHIFT, lineNumber, value);
            }
            previousToken = TOKEN_LESS;
            return makeToken(TOKEN_LESS, lineNumber, value);

        case '!':
            // check for = character
            if(*(source + 1) == '='){
                previousToken = TOKEN_BANG_EQUAL;
                source++;
                return makeToken(TOKEN_BANG_EQUAL, lineNumber, value);
            }
            previousToken = TOKEN_BANG;
            return makeToken(TOKEN_BANG, lineNumber, value);

        case '=':
            // check for = character
            if(*(source + 1) == '='){
                previousToken = TOKEN_EQUAL_EQUAL;
                source++;
                return makeToken(TOKEN_EQUAL_EQUAL, lineNumber, value);
            }
            previousToken = TOKEN_EQUAL;
            return makeToken(TOKEN_EQUAL, lineNumber, value);

        case '&':
            // TODO: check for &
            previousToken = TOKEN_BITWISE_AND;
            return makeToken(TOKEN_BITWISE_AND, lineNumber, value);

        case '|':
            // TODO: check for |
            previousToken = TOKEN_BITWISE_OR;
            return makeToken(TOKEN_BITWISE_OR, lineNumber, value);

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
