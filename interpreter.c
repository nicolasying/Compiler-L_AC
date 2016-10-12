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
void pushStack (int data, pileBase * pileTop) {
    pileBase * new = malloc(sizeof(pileBase));
    new->data = data;
    new->precedent = pileTop;
    pileTop = new;
}
int popStack (pileBase * pileTop) {
    pileBase * toDelete = pileTop;
    int data = toDelete->data;
    pileTop = pileTop->precedent;
    free(pileBase);
    return data;
}

pileBase data, type, retourne;

// definition temporaire de la table des symboles
int LAC[2000] = [0];
            // [0, 1, '+', 2, ENTIER, ENTIER, 1, ENTIER, 0, 1, // 9
            //  1, 4, 's', 'w', 'a', 'p', 2, ENTIER, ENTIER, 2, ENTIER, ENTIER, 2, 10, // 23
            //  2, 1, '.', 1, ENTIER, 0, 4, 22, // 28
            //  3, 5, '(', 'l', 'i', 't', ')', 0, 0, 6, 29, // 38
            //  4, 5, '(', 'f', 'i', 'n', ')', 0, 0, 8, 39, // 48
            //  5,  ];
int VM[200] = [0, 0, 0, 1, 0, 2, 0, 3, 0, 4];

void addFunctionBase (const int finIndVM, cosnt int indProcesseur, const int finIndLAC, char * name, int paraIn, int typeIn[], int paraOut, int typeOut[]) {
    // appending to LAC
    LAC[finIndLAC + 1] = LAC[LAC[finIndLAC]] + 1; // sequence number of the function in LAC
    LAC[finIndLAC + 2] = strlen(name);
    strncpy(LAC[finIndLAC + 3], name, LAC[finIndLAC + 2]);
    LAC[finIndLAC + 3 + LAC[finIndLAC + 2]] = paraIn;
    strncpy(LAC[finIndLAC + 4 + LAC[finIndLAC + 2]], typeIn, paraIn);
    LAC[finIndLAC + 4 + LAC[finIndLAC + 2] + paraIn] = paraOut;
    strncpy(LAC[finIndLAC + 5 + LAC[finIndLAC + 2] + paraIn], typeOut, paraOut);
    LAC[finIndLAC + 5 + LAC[finIndLAC + 2] + paraIn + paraOut] = finIndVM + 1// VM position;
    LAC[finIndLAC + 6 + LAC[finIndLAC + 2] + paraIn + paraOut] = finIndLAC + 1;

    //appending to VM
    VM[finIndVM + 1] = 0;
    VM[finIndVM + 2] = indProcesseur;
}

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

int isFunction (int i, lexeme_t * lexeme) {
    // this function searches through from the end of table des symboles, returns the index of the beginning of the matching function,
    // 0 if not.
    int length = strlen(lexeme[i].lex);
    int position = finLac;
    while (LAC[position + 1] != length && strncmp(lexeme[i].lex, (char *) LAC[position + 2], length) != 0 && position > 0) {
        position = LAC[LAC[position - 1]];
    }
    return position;
}

void compileur(int *VM, lexeme_t *lexemes, int t_c) { //t_c est le nombre de lexemes
    initialisation();

    int i = 0;
    do {
        if (lexemes[i].type == C) {
        } else {
            int posLAC;
            if ((posLac =isFunction(i)) > 0) {
                // the lexemes is a function, mock the calculation process

            } else {
                // function is not found, judgement of type of lexemes

            }
        }
    } while (i < t_c)
}