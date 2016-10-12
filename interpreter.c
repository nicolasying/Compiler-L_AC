//
//  Interprete
//  Compiler Lac
//
//  Created by Nicolas YING on 10/10/2016.
//  Copyright © 2016 Nicolas YING. All rights reserved.
//

#include "analex.h"
#include "analex.c"
#incldue "LACVM.c"
#include <string.h>
#include <stdlib.h>

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

void compilateur(lexeme_t *lexemes, int t_c) { //t_c est le nombre de lexemes
    initialisation();

    int i = 0;
    do {
        if (lexemes[i].type == C) {
        } else {
            int posLAC;
            if ((posLac =isFunction(i)) > 0) {
                // the lexemes is a function, mock the calculation process
                // check the availability of data/type stacks
                int paraIn = 
            } else {
                // function is not found, judgement of type of lexemes

                // assume the lexeme is an integer
                pushStack(atoi(lexemes[i].lex), data);
                pushStack(ENTIER, type);
            }
        }
        i++;
    } while (i < t_c)
}