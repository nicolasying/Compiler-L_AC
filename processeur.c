//
//  Processeur mockant,
//  une liste de fonctions soutenu par la VM, 
//  implémentée d'ailleurs, stimulée dans ce ficher, qui définie de plus l'ordre des fonctions
//  Compiler Lac
//
//  Created by Nicolas YING on 10/10/2016.
//  Copyright © 2016 Nicolas YING. All rights reserved.
//
//  Error code: 5**

// 0 lit (not available in interpreter mode)
// 1 str
// 2 fin (not available in interpreter mode)
// 3 .
// 4 \+
// 5 \-
// 6 \*
// 7 = (comparison)
// 8 dup
// 9 drop
// 10 swap
// 11 count
// 12 type
// 13 if (not available in interpreter mode)
// 14 else (not available in interpreter mode)
// 15 then (not available in interpreter mode)
// 20 && (optional functions from now on)
// 21 ||
// 22 !
// 23 inferior or equal
// 24 defer
// 25 recurse
// 26 '
// 27 is
// 28 calculate
// 29 catenate

#include "common_component.h"
#include "processor.h"
#include "Syntax_calculate/BNF_C.h"
#include <stdio.h>
#include <stdlib.h>

static basicStack ** data, ** type, ** retourne;
static int *VM;
static char *stringMem;

void linkProcessor(basicStack ** dataStack, basicStack ** typeStack, basicStack ** callStack, char * stringMemory, int * exVM) {
    data = dataStack;
    type = typeStack;
    retourne = callStack;
    stringMem = stringMemory;
    VM = exVM;
}

void lit(void) { // qui lit une element du pile, la stocker dans le registre
    int tmp = popStack(retourne); // position de lit
    pushStack(tmp + 1, retourne); // avance 1 indice
    pushStack(VM[tmp + 1], data);
    pushStack(ENTIER, type); // suppose
    printf("CPU: lit\n");
}

void str(void) { // to indicate that the next element in the array is a string
    return;
}

void fin(void) {
    popStack(retourne);
    #ifdef DEBUG
    printf("CPU: fin\n");
    #endif
}

void affichage(void) { // qui empile une element, l'affichier sur output
    int op1 = popStack(data), opt1 = popStack(type);
    printf("L_AC_RESULT:  %d, type: ", op1);
    switch (opt1) {
    case ENTIER:
        printf("integer\n");
        break;
    case ANY:
        printf("any\n");
        break;
    case BOOLEAN:
        printf("bool\n");
        break;
    case CHAINECHAR:
        printf("address of string\n");
        break;
    case FLOATANT:
        printf("float\n");
        break;
    case CHAINECHARNOHEADER:
        printf("address of a string without header\n");
        break; 
    default:
        printf("ERROR\n");
        break;
    }
}

void addition(void) {
    int op1 = popStack(data), opt1 = popStack(type);
    int op2 = popStack(data), opt2 = popStack(type);
    if (opt2 == opt1 && opt1 == ENTIER) {
        pushStack(op1 + op2, data);
        pushStack(ENTIER, type);
    } else {
        printf("CPU: add error\n");
        // exit(504);
        // reverting to previous state.
        pushStack(op2, data); pushStack(opt2, type);
        pushStack(op1, data); pushStack(opt1, type);
        return;
    }
    #ifdef DEBUG
    printf("CPU: add\n");
    #endif
}

void substraction(void) {
    int op1 = popStack(data), opt1 = popStack(type);
    int op2 = popStack(data), opt2 = popStack(type);
    if (opt2 == opt1 && opt1 == ENTIER) {
        pushStack(op2 - op1, data);
        pushStack(ENTIER, type);
    } else {
        printf("CPU: substraction error\n");
        // exit(505);
        // reverting to previous state.
        pushStack(op2, data); pushStack(opt2, type);
        pushStack(op1, data); pushStack(opt1, type);
    }
    #ifdef DEBUG
    printf("CPU: substraction\n");
    #endif
}

void multiplication(void) {
    int op1 = popStack(data), opt1 = popStack(type);
    int op2 = popStack(data), opt2 = popStack(type);
    if (opt2 == opt1 && opt1 == ENTIER) {
        pushStack(op1 * op2, data);
        pushStack(ENTIER, type);
    } else {
        printf("CPU: mul error\n");
        // exit(506);        
        // reverting to previous state.
        pushStack(op2, data); pushStack(opt2, type);
        pushStack(op1, data); pushStack(opt1, type);
    }
    #ifdef DEBUG
    printf("CPU: mul\n");
    #endif
}

void comparison(void) {
    int op1 = popStack(data), opt1 = popStack(type);
    int op2 = popStack(data), opt2 = popStack(type);
    if (opt2 == opt1) {
        pushStack(op1 == op2, data);
        pushStack(BOOLEAN, type);
    } else {
        printf("CPU: comparison error\n");
        // exit(507);        
        // reverting to previous state.
        pushStack(op2, data); pushStack(opt2, type);
        pushStack(op1, data); pushStack(opt1, type);
    }
    #ifdef DEBUG
    printf("CPU: comparison\n");
    #endif
}

void dup(void) {
    int op1 = popStack(data), opt1 = popStack(type);

    pushStack(op1, data);
    pushStack(opt1, type);
    pushStack(op1, data);
    pushStack(opt1, type);

    #ifdef DEBUG
    printf("CPU: dup\n");
    #endif
}

void drop(void) {
    popStack(data);
    popStack(type);

    #ifdef DEBUG
    printf("CPU: drop\n");
    #endif
}

void swap(void) {
    int op1 = popStack(data), opt1 = popStack(type);
    int op2 = popStack(data), opt2 = popStack(type);
    
    pushStack(op1, data); pushStack(opt1, type);
    pushStack(op2, data); pushStack(opt2, type);

    #ifdef DEBUG
    printf("CPU: swap\n");
    #endif // DEBUG
}

void count(void) {
    int op1 = popStack(data), opt1 = popStack(type);
    if(opt1 == CHAINECHAR) {
        int length = stringMem[op1];
        for (int i = 0; i <= length; i++) {
            stringMem[(op1 + i) % MAX_STRING_SIZE] = stringMem[(op1 + i + 1) % MAX_STRING_SIZE]; 
        }
        pushStack(op1, data); pushStack(CHAINECHARNOHEADER, type);
        pushStack(length, data); pushStack(ENTIER, type);
    } else {
        printf("CPU: count, type error. Poping out.\n");
    }

    #ifdef DEBUG
    printf("CPU: count\n");
    #endif // DEBUG
}

void typeProc(void) {
    int op1 = popStack(data), opt1 = popStack(type);
    int op2 = popStack(data), opt2 = popStack(type);
    if (opt1 == ENTIER && opt2 == CHAINECHARNOHEADER) {
        int i = 0;
        printf("L_AC_TYPE: ");
        while (i < op1) {
            printf("%c", stringMem[op2 + i++ % MAX_STRING_SIZE]);
        }
        printf("\n");
    } else {
        printf("CPU: type, type error. Poping out.\n");
    }

    #ifdef DEBUG
    printf("CPU: type\n");
    #endif // DEBUG
}

void calculate(void) {
    int op1 = popStack(data), opt1 = popStack(type);
    if (opt1 != CHAINECHAR) {
        printf("CPU: calculate, type error.\n");
    }
    int result = calculateString (&stringMem[op1 % MAX_STRING_SIZE]);
    pushStack(result, data); pushStack(ENTIER, type);
    #ifdef DEBUG
    printf("CPU: calculate\n");
    #endif // DEBUG
}

void def(void) {
    // compilateur();
    printf("CPU: def\n");
}