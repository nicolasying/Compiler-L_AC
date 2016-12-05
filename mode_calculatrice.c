//
//  Mode Calculatrice
//  Compiler Lac_reorg
//
//  Created by Nicolas YING on 06/12/2016.
//  Copyright © 2016 Nicolas YING. All rights reserved.
//
//  Error code: 0**

#include "mode_calculatrice.h"
#include "analyse_lexical.h"
#include "common_component.h"
#include "processeur.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG 

void initLacInterprete(int * symbolTable, int * VM) {
    // Cleaning the tables
    memset(symbolTable, 0, SYMBOL_TABLE_SIZE);
    memset(VM, 0, VM_SIZE);

    int posSymbol = 0, posVM = 0; // the last position of each table
    addBaseFunction (symbolTable, VM, &posSymbol, &posVM, 3, ".", 1, (int[]){ENTIER}, 0, (int[]){});
    addBaseFunction (symbolTable, VM, &posSymbol, &posVM, 4, "+", 2, (int[]){ENTIER, ENTIER}, 1, (int[]){ENTIER}); 
    addBaseFunction (symbolTable, VM, &posSymbol, &posVM, 5, "-", 2, (int[]){ENTIER, ENTIER}, 1, (int[]){ENTIER}); 
    addBaseFunction (symbolTable, VM, &posSymbol, &posVM, 6, "*", 2, (int[]){ENTIER, ENTIER}, 1, (int[]){ENTIER}); 
    addBaseFunction (symbolTable, VM, &posSymbol, &posVM, 7, "=", 2, (int[]){ENTIER, ENTIER}, 1, (int[]){BOOLEAN}); 
    addBaseFunction (symbolTable, VM, &posSymbol, &posVM, 8, "dup", 1, (int[]){ANY}, 2, (int[]){ANY, ANY}); 
    addBaseFunction (symbolTable, VM, &posSymbol, &posVM, 9, "drop", 1, (int[]){ANY}, 0, (int[]){}); 
    addBaseFunction (symbolTable, VM, &posSymbol, &posVM, 10, "swap", 2, (int[]){ANY, ANY}, 0, (int[]){}); 
    addBaseFunction (symbolTable, VM, &posSymbol, &posVM, 11, "count", 1, (int[]){CHAINECHAR}, 2, (int[]){CHAINECHARNOHEADER, ENTIER}); 
    addBaseFunction (symbolTable, VM, &posSymbol, &posVM, 12, "type", 2, (int[]){CHAINECHARNOHEADER, ENTIER}, 0, (int[]){}); 
    addBaseFunction (symbolTable, VM, &posSymbol, &posVM, 28, "calculate", 1, (int[]){CHAINECHAR}, 0, (int[]){}); 
}

int main(int argc, char * argv[]) { // it should not take any input
    #ifdef DEBUG
    printf("Running in DEBUG mode.\n");
    #endif // DEBUG

    // Initialisation of environment
    char lineBuffer[MAX_LINE_LENGTH] = {0};
    lexeme_Element lexemeList[MAX_LEXEME_NUMBER];
    basicStack *data, *type, *call;
    int symbolTable[SYMBOL_TABLE_SIZE] = {0}, VM[VM_SIZE] = {0}, stringMem[MAX_STRING_SIZE] = {0};
    int posMemory = 0, posSymbol = 0, posVM = 0;
    basicFunc processor[PROCESS_FUN_QUAN];
    linkProcessor(data, type, call, stringMem, VM);

    // Initialisation of L_ac components
    data = malloc(sizeof(basicStack));
    type = malloc(sizeof(basicStack));
    data->data = 0;
    data->precedent = NULL;
    type->data = 0;
    type->precedent = NULL;
    initLacInterprete(symbolTable, VM);

    while(1) {
        // Take input from the shell
        printf("Shell nicolasien:> ");
        gets(lineBuffer);
        
        // Pass to lexcial analysis, which is a common component
        analyseLexical(lineBuffer, lexemeList);

        memset(lineBuffer, 0, MAX_LINE_LENGTH);
    }

    return 0;
}


