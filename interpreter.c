//
//  Interprete
//  Compiler Lac
//
//  Created by Nicolas YING on 10/10/2016.
//  Copyright © 2016 Nicolas YING. All rights reserved.
//

#include "analex.h"
#include "analex.c"

#define ENTIER 1
#define ANY 0
#define BOOLEAN 2
#define CHAINECHAR 3

typedef struct pileBase pileBase;
struct pileBase // la première element dans la pile seront toujours négligé
{
    int data;
    pileBase * precedent;
    pileBase * suivant;
};
void push (int data, pileBase * pileTop) {

}
int pop (pileBase * pileTop) {

}

pileBase data, type;

// definition temporaire de la table des symboles
int LAC[] = [0, 1, '+', 2, ENTIER, ENTIER, 1, ENTIER, /?/, 1, 4, 's', 'w', 'a', 'p', 2, ENTIER, ENTIER, 2, ENTIER, ENTIER, /?/];
int VM[] = [0, 0];

void (*foncBase)(void) processeur[] = [, ]

void initialisation () { 
    // initialisation des piles
    data.data = 0;
    data.precedent = NULL;
    data.suivant = NULL;
    type.data = 0;
    type.precedent = NULL;
    type.suivant = NULL;
}

void executeur (int * VM, lexeme_t * lexemes) {
    
}