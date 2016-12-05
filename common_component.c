//
//  Common component
//  Compiler Lac_reorg
//
//  Created by Nicolas YING on 07/12/2016.
//  Copyright © 2016 Nicolas YING. All rights reserved.
//
//  contains structures for both modes
//  Error code: 9**

#include "common_component.h"
#include "processeur.c"
#include <stdlib.h>
#include <stdio.h>

void initProcessor (basicFunc * processor) {
    processor[0] = &lit;
    processor[2] = &fin;
    processor[3] = &affichage;
    processor[4] = &addition;
    processor[6] = &multiplication;
    processor[10] = &swap;
    processor[20] = &def;
}

void addBaseFunction (const int * symbolTable, const int * VM, int *posSymbol, int* posVM, const int processorIndex, char* name, int paraIn, int typeIn[], int paraOut, int typeOut[]) {
    
    if (*posSymbol >= SYMBOL_TABLE_SIZE || *posVM >= VM_SIZE) {
        printf("Symbol table or VM table full.");
        exit(910)
    }
    
    // Adding info to symbol table
    symbolTable[*posSymbol + 1] = findStringLength(name);
    for (int i = 0; i < basicStacksymbolTable[*posSymbol + 1]; i++) {
        basicStacksymbolTable[*posSymbol + 2 + i] = name[i];
    }
    symbolTable[*posSymbol + 2 + symbolTable[*posSymbol + 1]] = paraIn;
    for (int i = 0; i < paraIn; i++) {
        symbolTable[*posSymbol + 3 + i + symbolTable[*posSymbol + 1]] = typeIn[i];
    }
    symbolTable[*posSymbol + 3 + symbolTable[*posSymbol + 1] + paraIn] = paraOut;
    for (int i = 0; i < paraOut; i++) {
        symbolTable[*posSymbol + 4 + symbolTable[*posSymbol + 1] + paraIn + i] = typeOut[i];
    }
    symbolTable[*posSymbol + 4 + symbolTable[*posSymbol + 1] + paraIn + paraOut] = *posVM; // VM position;
    symbolTable[*posSymbol + 5 + symbolTable[*posSymbol + 1] + paraIn + paraOut] = *posSymbol + 1;
   
    *posSymbol += 5 + symbolTable[*posSymbol + 1] + paraIn + paraOut;

    // Adding info to VM
    VM[(*posVM)++] = 0; // Fonction de base
    VM[(*posVM)++] = processorIndex;
}

void pushStack (int data, basicStack ** topNode) {
    basicStack * pile = * topNode;
    basicStack * new = malloc(sizeof(basicStack));
    if(new == NULL) {
        printf("Common component: \nMemory allocation failed.\n");
        exit(900);
    }
    new->data = data;
    new->precedent = pile;
    *topNode = new;
}

int popStack (basicStack ** topNode) {
    basicStack * toDelete = *topNode;
    if(toDelete->precedent == NULL) exit(901); // this is the bottom
    int data = toDelete->data;
    *topNode = toDelete->precedent;
    free(toDelete);
    return data;
}

void displayStack (const basicStack ** dataStack, const basicStack ** typeStack, const int * memorySpace) {
    printf("Stack : (top) ");
    basicStack * tempPointerD = *dataStack, *tempPointerT = *typeStack;
    while(tempPointerD->precedent != NULL) {
        if(tempPointerT->data == ENTIER) printf("%d > ", tempPointerD->data);
        else if(tempPointerT->data == BOOLEAN) printf("%d > ", tempPointerD->data);
        else if(tempPointerT->data == CHAINECHAR) {
            printString(memorySpace, tempPointerT->data); 
            printf(" > ");
        }
        else if(tempPointerT->data == CHAINECHARNOHEADER) printf("[A String with no header] > ", tempPointerD->data);
    } 
    printf("(bottom)\n");
}

void printString(int * memorySpace, int position) {
    int length = memorySpace[position];
    while (length-- > 0) {
        printf("%c", memorySpace[++position]);
    }
}

int findStringLength (char * string) {
    int i = 0;
    while (lex[i] != '\0') {
        i++;
    }
    return i;
}