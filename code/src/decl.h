#ifndef SEA_DECL_H
#define SEA_DECL_H
#include "defs.h"

ObjString* allocateString(char* chars, int length);
void initIntArray(IntArray* intArray, int initialCapacity);
void freeIntArray(IntArray* intArray);
void pushIntArray(IntArray* intArray, uint32_t intValue);
uint32_t peekIntArray(IntArray* intArray);
uint32_t popIntArray(IntArray* intArray);
void initValueArray(ValueArray* valueArray, int initialCapacity);
void freeValueArray(ValueArray* valueArray);
uint32_t pushValue(ValueArray* valueArray, Value value);
Value* peekValueArray(ValueArray* valueArray);
Value popValue(ValueArray* valueArray);
void initScanner(const char* buffer);
Token scanToken();
void initTable(Table* table);
Bool tableSet(Table* table, ObjString* key, Value* value);
// returns value in value* pointer
Bool tableGet(Table* table, ObjString* key, Value* value);
Bool tableDelete(Table* table, ObjString* key);
void freeTable(Table* table);
void tableUpdate(Table* table, ObjString* key, Value* value);
void debugTable(Table* table, int printDist);
void initVm(VM* vm, char* source);
void freeVm(VM* vm);

void interpret(VM* vm);

// parser.c
int match(int tokenType);
int matchs(int tokenType);
int matchsr(int tokenType, const char* whatMsg);
int matchr(int tokenType, const char* whatMsg);
void wrongm(int tokenType, const char* whatMsg);
void scan_into();
int m_semi();
int m_eof();

//-----

void expression();
void statements();
void emit(uint32_t op, int isIndex);
void emit2(uint32_t op1, uint32_t op2, int isIndex);
void compile(char *buffer);
int findIdentifier(const char* identifierName, int length);


// sym_table.c
void initSymbolTable(SymbolTable* symbolTable, uint32_t initialCapacity);
void freeSymbolTable(SymbolTable* symbolTable);
int addSymbol(SymbolTable* symbolTable, Symbol* symbol);
int getSymbol(SymbolTable* symbolTable, int direction, const char* symbolName, int length);
void deleteSymbols(SymbolTable* symbolTable, int count);
#endif
