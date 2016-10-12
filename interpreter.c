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
    free(toDelete);
    return data;
}

pileBase data, type, retourne;

// definition temporaire de la table des symboles
int LAC[2000] = {0};
            // [0, 1, '+', 2, ENTIER, ENTIER, 1, ENTIER, 0, 1, 
            //     4, 's', 'w', 'a', 'p', 2, ENTIER, ENTIER, 2, ENTIER, ENTIER, 2, 10, 
            //     1, '.', 1, ENTIER, 0, 4, 22, 
            //     5, '(', 'l', 'i', 't', ')', 0, 0, 6, 29, 
            //     5, '(', 'f', 'i', 'n', ')', 0, 0, 8, 39, 
            //      ];
int VM[200] = {0};
static int finIndLAC = 0, finIndVM = -1;
void addFunctionBase (const int indProcesseur, char * name, int paraIn, int typeIn[], int paraOut, int typeOut[]) {
    // appending to LAC
    // LAC[finIndLAC + 1] = LAC[LAC[finIndLAC]] + 1; // sequence number of the function in LAC
    LAC[finIndLAC + 1] = strlen(name);
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
    LAC[finIndLAC + 4 + LAC[finIndLAC + 1] + paraIn + paraOut] = finIndVM + 1; // VM position;
    LAC[finIndLAC + 5 + LAC[finIndLAC + 1] + paraIn + paraOut] = finIndLAC + 1;
   
    finIndLAC = finIndLAC + 5 + LAC[finIndLAC + 1] + paraIn + paraOut;

    //appending to VM
    VM[finIndVM++] = 0;
    VM[finIndVM++] = indProcesseur;
}

void initLACVM (void) {
    int typeIn[] = {ENTIER, ENTIER}, typeOut[] = {ENTIER};
    addFunctionBase(0, "+", 2, typeIn, 1, typeOut);
}

//void (*foncBase)(void) processeur[] = [, ]

void makeEmpty () { // comme le nom indique
   while (data.precedent != NULL) popStack(&data);
   while (type.precedent != NULL) popStack(&type);
   while (retourne.precedent != NULL) popStack(&retourne);
}
void initialisation () { 
    makeEmpty();
    // initialisation des piles
    data.data = 0;
    data.precedent = NULL;
    type.data = 0;
    type.precedent = NULL;
}

int isFunction (int i, lexeme_t * lexeme) {
    // this function searches through from the end of table des symboles, returns the index of the beginning of the matching function,
    // 0 if not.
    int length = strlen(lexeme[i].lex);
    int position = finIndLAC;
    while (LAC[position + 1] != length && position > 0) {
        // firstly, need to compare two strings
        int j = 0;
        while (j < length) {
            if (lexeme[i].lex[j] != LAC[position + 1 + j]) {
                break;
            } else {
                j++;
            }
        }
        if (j == length) return position;
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
            if ((posLAC =isFunction(i, lexemes)) > 0) {
                // the lexemes is a function, mock the calculation process
                // check the availability of data/type stacks
//                int paraIn = 
            } else {
                // function is not found, judgement of type of lexemes
                //int data = type = 0;
                // assume the lexeme is an integer
                pushStack(atoi(lexemes[i].lex), &data);
                pushStack(ENTIER, &type);
            }
        }
        i++;
    } while (i < t_c);
}