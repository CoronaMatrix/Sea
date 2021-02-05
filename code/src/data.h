
//#ifndef extern_
//#define extern_ extern
//#endif
#ifndef SEA_DATA_H
#define SEA_DATA_H
#include "defs.h"
#include <stdint.h>
extern Token currentToken;
extern CompiledChunk compiledChunk;
extern IntArray opStack;
extern IntArray indexes;
extern int slotNumber;
extern int scopeDepth;
extern SymbolTable symTable;
#endif
