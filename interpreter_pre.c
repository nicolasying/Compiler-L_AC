//
//  interprete_pre.c
//  Compiler Lac
//
//  Created by Nicolas YING on 03/12/2016.
//  Copyright © 2016 Nicolas YING. All rights reserved.
//
//
//  This file checks if the symbol table / VM for interpreter mode is ready,
//  if not, it will generate one.

#include "analex.h"
#include "processeur.c"
#include "interpreter.h"
#include <string.h>
#include <stdlib.h>

baseFonc processeur[MAX_PROC_FUNCS];

int posFonLITVM, posFonFINVM; // lit & fin position in VM

// definition temporaire de la table des symboles
int LAC[MAX_SYMBOL_NUMBER] = {0};
int VM[MAX_VM_NUMBER] = {0};
int finIndLAC = 0, finIndVM = 6;

void addFunctionBase (const int indProcesseur, char * name, int paraIn, int typeIn[], int paraOut, int typeOut[]) {
    // appending to LAC
    // LAC[finIndLAC + 1] = LAC[LAC[finIndLAC]] + 1; // sequence number of the function in LAC
    LAC[finIndLAC + 1] = lexlen(name);
    for (int i = 0; i < LAC[finIndLAC + 1]; i++) {
        LAC[finIndLAC + 2 + i] = name[i];
    }
    LAC[finIndLAC + 2 + LAC[finIndLAC + 1]] = paraIn;
    for (int i = 0; i < paraIn; i++) {
        LAC[finIndLAC + 3 + i + LAC[finIndLAC + 1]] = typeIn[i];
    }
    LAC[finIndLAC + 3 + LAC[finIndLAC + 1] + paraIn] = paraOut;
    for (int i = 0; i < paraOut; i++) {
        LAC[finIndLAC + 4 + LAC[finIndLAC + 1] + paraIn + i] = typeOut[i];
    }
    LAC[finIndLAC + 4 + LAC[finIndLAC + 1] + paraIn + paraOut] = finIndVM; // VM position;
    LAC[finIndLAC + 5 + LAC[finIndLAC + 1] + paraIn + paraOut] = finIndLAC + 1;
   
    finIndLAC += 5 + LAC[finIndLAC + 1] + paraIn + paraOut;

    // appending to VM, with 4 indices reserved to fin and lit.
    if (finIndVM + 2 >= MAX_VM_NUMBER - 4) {
        printf("VM is full. Extent memory for VM.\n");
        exit(311);
    }
    VM[finIndVM++] = 0;
    VM[finIndVM++] = indProcesseur;
    
    // for (int j = LAC[finIndLAC]; j < finIndLAC; j++)
    // {
    //     printf("%d ", LAC[j]);
    // }
}

void initLACVMPro (void) {

    addFunctionBase(4, "+", 2, (int[]){ENTIER, ENTIER}, 1, (int[]){ENTIER}); 
    addFunctionBase(6, "*", 2, (int[]){ENTIER, ENTIER}, 1, (int[]){ENTIER}); 
    addFunctionBase(10, "swap", 2, (int[]){ENTIER, ENTIER}, 0, (int[]){});
    addFunctionBase(3, ".", 1, (int[]){ENTIER}, 0, (int[]){});
    addFunctionBase(20, ":", 0, (int[]){}, 0, (int[]){});
    processeur[3] = &affichage;
    processeur[4] = &addition;
    processeur[10] = &swap;
    processeur[20] = &def;
    processeur[6] = &multiplication;
    processeur[2] = &fin;
    processeur[0] = &lit;
    posFonFINVM = 0;
    posFonLITVM = 4;
    VM[posFonFINVM] = 0;
    VM[posFonFINVM + 1] = 4;
    VM[posFonLITVM] = 0;
    VM[posFonLITVM + 1] = 0;
}

void precheck(int version) {
    
}