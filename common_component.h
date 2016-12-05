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

#define MAX_STRING_SIZE 500
#define SYMBOL_TABLE_SIZE 500
#define VM_SIZE 300
#define PROCESS_FUN_QUAN 40

// About processor, symbol table and VM
typedef void (*basicFunc)(void);
void initProcessor(basicFunc * processor);
void addBaseFunction (const int * symbolTable, const int * VM, int *posSymbol, int* posVM, const int processorIndex, char* name, int paraIn, int typeIn[], int paraOut, int typeOut[]);

// About stacks
typedef struct basicStack basicStack;
struct basicStack {
// The first element in the stack (bottom) is always to be neglected
    int data;
    basicStack * precedent;
};

void pushStack (int data, basicStack ** topNode);
int popStack (basicStack ** topNode);
void displayStack (const basicStack ** dataStack, const basicStack ** typeStack, const int * memorySpace);

// L_ac types
#define ENTIER 1
#define ANY 0
#define BOOLEAN 2
#define CHAINECHAR 3
#define FLOATANT 4
#define CHAINECHARNOHEADER 5

// About strings
void printString(int * memorySpace, int position);
int findStringLength(char * string);

#endif // !COMMON_COMPONENT