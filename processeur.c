//
//  Processeur mockant,
//  une liste de fonctions supporté par la VM, 
//  implémentée d'ailleurs, stimulée dans ce ficher, qui définie de plus l'ordre des fonctions
//  Compiler Lac
//
//  Created by Nicolas YING on 10/10/2016.
//  Copyright © 2016 Nicolas YING. All rights reserved.
//

// struct pileBase // la première element dans la pile seront toujours négligé
// {
//     int data;
//     pileBase * precedent;
// };

#include "interpreter.h"

extern pileBase * data, * type, * retourne;
// pileBase data, type, retourne;

void addition(void) {
    int op1 = popStack(&data), opt1 = popStack(&type);
    int op2 = popStack(&data), opt2 = popStack(&type);
    pushStack(op1 + op2, &data);
    pushStack(typeConversion(opt1, opt2), &type);
}

// implement as adiition + oppose ?
void substraction(void) {
    int op1 = popStack(&data), opt1 = popStack(&type);
    int op2 = popStack(&data), opt2 = popStack(&type);
    pushStack(op1 - op2, &data);
    pushStack(typeConversion(opt1, opt2), &type);
}

void swap(void) {
    int op1 = popStack(&data), opt1 = popStack(&type);
    int op2 = popStack(&data), opt2 = popStack(&type);
    pushStack(op2, &data); pushStack(opt2, &type);
    pushStack(op1, &data); pushStack(opt1, &type);
}

void affichage(void) { // qui empile une element, l'affichier sur output
    int op1 = popStack(&data), opt1 = popStack(&type);
    printf("Output is: %d,\n type: %d\n", op1, opt1);
}

void lit(void) { // qui lit une element du pile, la stocker dans le registre

}

void fin(void) {

}