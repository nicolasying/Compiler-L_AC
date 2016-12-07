//
//  Common component Header
//  Compiler Lac_reorg
//
//  Created by Nicolas YING on 07/12/2016.
//  Copyright © 2016 Nicolas YING. All rights reserved.
//
//  contains structures for both modes

#ifndef COMMON_COMPONENT
#define COMMON_COMPONENT

#include "analyse_lexical.h"

#define DEBUG // debugging flag

#define MAX_LINE_LENGTH 1000
#define MAX_LEXEME_NUMBER 300

#define MAX_STRING_SIZE 500
#define SYMBOL_TABLE_SIZE 500
#define VM_SIZE 300
#define PROCESS_FUN_QUAN 40

// About processor, symbol table and VM
typedef void (*basicFunc)(void);
void initProcessor(basicFunc * processor);
void addBaseFunction (int * symbolTable, int * VM, int *posSymbol, int* posVM, const int processorIndex, char* name, int paraIn, int typeIn[], int paraOut, int typeOut[]);
int findFunction (int sizeSymbolTable, int * symbolTable, lexeme_Element * lexeme, char * code); // search in symbol table if the lexeme is a funtion defined, returning the beginning position in symbol table, 0 if not found

// About stacks
typedef struct basicStack basicStack;
struct basicStack {
// The first element in the stack (bottom) is always to be neglected
    int data;
    basicStack * precedent;
};

void pushStack (int data, basicStack ** topNode);
int popStack (basicStack ** topNode);
void displayStack (basicStack ** dataStack, basicStack ** typeStack, int * memorySpace);
void clearStack (basicStack ** topNode);

// L_ac types
#define ENTIER 1
#define ANY 0
#define BOOLEAN 2
#define CHAINECHAR 3
#define FLOATANT 4
#define CHAINECHARNOHEADER 5

#define LTRUE 8901
#define LFALSE 8900

// About strings
void printString(int * memorySpace, int position); // print a LAC string in a safe way
int findStringLength(char * string);
int convertLexeme2Number (char * code, lexeme_Element * lexeme, int * number); 
void strcpyMEM(int * stringMem, const int * scr, int * posMem); // copy a LAC string into memory safely
void strncpyMEM(int * stringMem, const int * scr, int * posMem, int length); // copy a C string(int) into memory safely
void cstrncpyMEM(int * stringMem, const char * scr, int * posMem, int length); // copy a C string(char) into memory safely

#endif // !COMMON_COMPONENT