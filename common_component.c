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
#include "processor.h"
#include <stdlib.h>
#include <stdio.h>

void initProcessor (basicFunc * processor) {
    processor[0] = &lit;
    processor[1] = &str;
    processor[2] = &fin;
    processor[3] = &affichage;
    processor[4] = &addition;
    processor[5] = &substraction;
    processor[6] = &multiplication;
    processor[7] = &comparison;
    processor[8] = &dup;
    processor[9] = &drop;
    processor[10] = &swap;
    processor[11] = &count;
    processor[12] = &typeProc;
    processor[13] = &fif;
    processor[14] = &felse;
    processor[15] = &fthen;
    processor[20] = &land;
    processor[21] = &lor;
    processor[22] = &lnot;
    processor[23] = &smaller;
    // processor[24] = &defer;
    processor[25] = &recurse;
    // processor[26] = &prime;
    // processor[27] = &is;
    processor[28] = &calculate;
    processor[29] = &catenate;
}

void addBaseFunction (int * symbolTable, int * VM, int *posSymbol, int* posVM, const int processorIndex, char* name, int paraIn, int typeIn[], int paraOut, int typeOut[]) {
    
    if (*posSymbol >= SYMBOL_TABLE_SIZE || *posVM >= VM_SIZE) {
        printf("Symbol table or VM table full.");
        exit(910);
    }
    
    // Adding info to symbol table
    symbolTable[*posSymbol + 1] = findStringLength(name);
    for (int i = 0; i < symbolTable[*posSymbol + 1]; i++) {
        symbolTable[*posSymbol + 2 + i] = name[i];
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

int findFunction (int sizeSymbolTable, int * symbolTable, lexeme_Element * lexeme, char * code) {
    int length = lexeme->end - lexeme->begin + 1; 
    // length of the lexeme used to quick matching
    int position = symbolTable[sizeSymbolTable]; 
    // position is the last symbol defined in the table

    #ifdef DEBUG
    printf("findFunction: searching for : ");
    int i = 0;
    while(i < length) {
        printf("%c",code[lexeme->begin + i++]);
    }
    printf("\n");
    #endif // DEBUG

    while (position > 0) {
        if (symbolTable[position] != length) {
            // if the length doesn't match
            position = symbolTable[position];
        } else {
            // otherwise, compare two strings
            int j = 0;
            while (j < length) {
                if (code[lexeme->begin + j] != symbolTable[position + 1 + j]) {
                    break;
                    // resulting to a j < length, which fails the following examination
                } else {
                    j++;
                }
            }
            if (j == length) return position;
            // the symbol is found
            position = symbolTable[position - 1];
            // otherwise, try the next symbol
        }
    }
    return position;
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
    if(toDelete->precedent == NULL) {
        printf("Error 901: Poping empty stack. The program will crash.\n");
        exit(901); // this is the bottom
    }
    int data = toDelete->data;
    *topNode = toDelete->precedent;
    free(toDelete);
    return data;
}

void displayStack (basicStack ** dataStack, basicStack ** typeStack, int * memorySpace) {
    printf("Stack : (top) ");
    basicStack * tempPointerD = *dataStack, *tempPointerT = *typeStack;
    while(tempPointerD->precedent != NULL) {
        if(tempPointerT->data == ENTIER) printf("%d > ", tempPointerD->data);
        else if(tempPointerT->data == BOOLEAN) {
            if (tempPointerD->data == LFALSE) printf("FALSE > ");
            else printf("TRUE > ");
        }
        else if(tempPointerT->data == CHAINECHAR) {
            printf("\"");
            printString(memorySpace, tempPointerD->data); 
            printf("\" > ");
        }
        else if(tempPointerT->data == CHAINECHARNOHEADER) printf("[A String with no header] > ");
        tempPointerD = tempPointerD->precedent;
        tempPointerT = tempPointerT->precedent;
    } 
    printf("(bottom)\n");
}

void clearStack (basicStack ** topNode) {
    while ((*topNode)->precedent != NULL) popStack(topNode);
    free(*topNode);
}

void printString(int * memorySpace, int position) {
    int length = memorySpace[position % MAX_STRING_SIZE];
    while (length-- > 0) {
        printf("%c", memorySpace[++position % MAX_STRING_SIZE]);
    }
}

int findStringLength (char * string) {
    int i = 0;
    while (string[i] != '\0') {
        i++;
    }
    return i;
}

int convertLexeme2Number (char * code, lexeme_Element * lexeme, int * number) {
    int pos = lexeme->begin;
    *number = 0;
    while (pos <= lexeme->end) {
        int bitCheck = code[pos++] - '0';
        if (bitCheck > 9 || bitCheck < 0) {
            printf("Unregistered lexeme. Trying converting lexeme to number: invalid number.\n");
            return 1;
        }
        *number *= 10;
        *number += bitCheck;
    }
    return 0;
}

void strcpyMEM(int * stringMem, const int * scr, int * posMem) {
    int length = scr[0], i = 0;
    while (i <= length) {
        stringMem[(*posMem + i) % MAX_STRING_SIZE] = scr[i];
        i++;
    }
    *posMem += length + 1;
}

void strncpyMEM(int * stringMem, const int * scr, int * posMem, int length) {
    int i = 0;
    stringMem[(*posMem) % MAX_STRING_SIZE] = length;
    while (i < length) {
        stringMem[(*posMem + 1 + i) % MAX_STRING_SIZE] =  scr[i];
        i++;
    }
    *posMem += length + 1;
}

void cstrncpyMEM(int * stringMem, const char * scr, int * posMem, int length) {
    int i = 0;
    stringMem[(*posMem) % MAX_STRING_SIZE] = length;
    while (i < length) {
        stringMem[(*posMem + 1 + i) % MAX_STRING_SIZE] =  (int)scr[i];
        i++;
    }
    *posMem += length + 1;
}