//
//  Interprete
//  Compiler Lac
//
//  Created by Nicolas YING on 10/10/2016.
//  Copyright © 2016 Nicolas YING. All rights reserved.
//

#include "analex.h"
#include "analex.c"
#include <string.h>

#define ENTIER 1
#define ANY 0
#define BOOLEAN 2
#define CHAINECHAR 3

// definition des piles
typedef struct pileBase pileBase;
struct pileBase // la première element dans la pile seront toujours négligé
{
    int data;
    pileBase * precedent;
};
void push (int data, pileBase * pileTop) {
    pileBase * new = malloc(sizeof(pileBase));
    new->data = data;
    new->precedent = pileTop;
    pileTop = new;
}
int pop (pileBase * pileTop) {
    pileBase * toDelete = pileTop;
    int data = toDelete->data;
    pileTop = pileTop->precedent;
    free(pileBase);
    return data;
}

pileBase data, type, retourne;

// definition temporaire de la table des symboles
int LAC[] = [0, 1, '+', 2, ENTIER, ENTIER, 1, ENTIER, 0, 1, // 9
             1, 4, 's', 'w', 'a', 'p', 2, ENTIER, ENTIER, 2, ENTIER, ENTIER, 2, 10, // 23
             2, 1, '.', 1, ENTIER, 0, 4, 22, // 28
             3, 5, '(', 'l', 'i', 't', ')', 0, 0, 6, 29, // 38
             4, 5, '(', 'f', 'i', 'n', ')', 0, 0, 8, 39, // 48
             5,  ];
int VM[] = [0, 0, 0, 1, 0, 2, 0, 3, 0, 4];

static int finLAC = 39;

void (*foncBase)(void) processeur[] = [, ]

void makeEmpty () { // comme le nom indique
   while (data.precedent != NULL) pop(&data);
   while (type.precedent != NULL) pop(&type);
   while (retourne.precedent != NULL) pop(&retourne);
}
void initialisation () { 
    makeEmpty();
    // initialisation des piles
    data.data = 0;
    data.precedent = NULL;
    data.suivant = NULL;
    type.data = 0;
    type.precedent = NULL;
    type.suivant = NULL;
}

int isFunction (int i, lexeme_t * lexemes) {
    int length = strlen(lexemes[i]);
    int position = finLac;
    while (LAC[position + 1] != length && ) {

    }
}

void compileur(int *VM, lexeme_t *lexemes, int t_c) { //t_c est le nombre de lexemes
    initialisation();

    int i = 0;
    do {
        if (lexemes[i].type == C) {
        } else {
            if (isFunction(i)) {
                // mock the calculation process
            } else {
                // judgement of type of lexemes
            }
        }
    } while (i < t_c)
}