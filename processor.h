//
//  Processeur mockant HEADER
//  une liste de fonctions soutenu par la VM, 
//  implémentée d'ailleurs, stimulée dans ce ficher, qui définie de plus l'ordre des fonctions
//  Compiler Lac
//
//  Created by Nicolas YING on 06/12/2016.
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

#ifndef PROCESSOR
#define PROCESSOR

#include "common_component.h"

void linkProcessor(basicStack ** dataStack, basicStack ** typeStack, basicStack ** callStack, int * stringMemory, int * exVM, int * exPosMem);
void lit(void);
void str(void);
void fin(void);
void affichage(void);
void addition(void);
void substraction(void);
void multiplication(void);
void comparison(void);
void dup(void);
void drop(void);
void swap(void);
void count(void);
void typeProc(void);
void fif(void);
void felse(void);
void fthen(void);
void land(void);
void lor(void);
void lnot(void);
void smaller(void);
void recurse(void);
void calculate(void);
void catenate(void);

#endif // !PROCESSOR