//
//  Interprete
//  Compiler Lac
//
//  Created by Nicolas YING on 10/10/2016.
//  Copyright © 2016 Nicolas YING. All rights reserved.
//

#include "analex.h"
#include "processeur.c"
#include "interpreter.h"
#include <string.h>
#include <stdlib.h>

// function de conversion de type 
int typeConversion(int typ1, int typ2) {
    if (typ1 == typ2) {
        return typ1;
    } else if (typ1 > typ2) {
        typ2 = typ2 - typ1;
        typ1 = typ2;
        typ2 = typ1 - typ2;
    }
    // now typ1 is smaller than typ2, begin rule list matching
    switch (typ1) {
        case 0:
            return typ2;
        case 1:
            if (typ2 == 4) { return typ2; }
            else { printf("Type imcompatible. \n"); exit(301); }
        default:
            printf("Type imcompatible. \n"); 
            exit(301);
            break;
    }
}

void pushStack (int data, pileBase ** pileTop) {
    pileBase * pile = * pileTop;
    pileBase * new = malloc(sizeof(pileBase));
    new->data = data;
    new->precedent = pile;
    * pileTop = new;
}
int popStack (pileBase ** pileTop) {
    pileBase * toDelete = * pileTop;
    if(toDelete->precedent == NULL) exit(302);
    int data = toDelete->data;
    *pileTop = toDelete->precedent;
    free(toDelete);
    return data;
}

pileBase *data, *type, *retourne;
baseFonc processeur[20];
int posLex;
const int posFonLITVM = 19, posFonFINVM = 20; // lit position in VM, numbre est faux

// definition temporaire de la table des symboles
int LAC[MAX_SYMBOL_NUMBER] = {0};
            // [0, 1, '+', 2, ENTIER, ENTIER, 1, ENTIER, 0, 1, 
            //     4, 's', 'w', 'a', 'p', 2, ENTIER, ENTIER, 2, ENTIER, ENTIER, 2, 10, 
            //     1, '.', 1, ENTIER, 0, 4, 22, 
            //     5, '(', 'l', 'i', 't', ')', 0, 0, 6, 29, 
            //     5, '(', 'f', 'i', 'n', ')', 0, 0, 8, 39, 
            //      ];
int VM[MAX_VM_NUMBER] = {0};
static int finIndLAC, finIndVM;

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
    LAC[finIndLAC + 4 + LAC[finIndLAC + 1] + paraIn + paraOut] = finIndVM; // VM position;
    LAC[finIndLAC + 5 + LAC[finIndLAC + 1] + paraIn + paraOut] = finIndLAC + 1;
   
    finIndLAC += 5 + LAC[finIndLAC + 1] + paraIn + paraOut;

    //appending to VM
    VM[finIndVM++] = 0;
    VM[finIndVM++] = indProcesseur;
    
    // for (int j = LAC[finIndLAC]; j < finIndLAC; j++)
    // {
    //     printf("%d ", LAC[j]);
    // }
}

void initLACVMPro (void) {

    addFunctionBase(1, "+", 2, (int[]){ENTIER, ENTIER}, 1, (int[]){ENTIER}); 
    addFunctionBase(5, "swap", 2, (int[]){ENTIER, ENTIER}, 0, (int[]){});
    addFunctionBase(0, ".", 1, (int[]){ENTIER}, 0, (int[]){});
    processeur[0] = &affichage;
    processeur[1] = &addition;
    processeur[5] = &swap;
    posFonFINVM = MAX_VM_NUMBER - 1;
    posFonLITVM = MAX_VM_NUMBER;
}


void makeEmpty () { // comme le nom indique
   while (data->precedent != NULL) popStack(&data);
   while (type->precedent != NULL) popStack(&type);
   while (retourne->precedent != NULL) popStack(&retourne);
}
void initialisation () { 
    // initialisation des piles
    data = malloc(sizeof(pileBase));
    type = malloc(sizeof(pileBase));
    retourne = malloc(sizeof(pileBase));
    data->data = 0;
    data->precedent = NULL;
    type->data = 0;
    type->precedent = NULL;
}

int isFunction (int i, lexeme_t * lexeme) {
    // this function searches through from the end of table des symboles, returns the index of the beginning of the matching function,
    // 0 if not.
    int length = strlen(lexeme[i].lex);
    int position = LAC[finIndLAC];
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
        position = LAC[position - 1];
    }
    return position;
}

void interpreteur(lexeme_t *lexemes, int t_c) { //t_c est le nombre de lexemes
    initialisation();
    posLex = 0;
    do {
        if (lexemes[posLex].type == C) {
        } else {
            int posLAC;
            if ((posLAC = isFunction(posLex, lexemes)) > 0) {
                // the lexemes is a function, mock the calculation process
                // check the availability of data/type stacks
                int lenName = LAC[posLAC];
                int paraIn = LAC[posLAC + lenName + 1];
                int paraOut = LAC[posLAC + lenName + paraIn + 2];
                int posVM = LAC[posLAC + lenName + paraIn + paraOut + 3];
                if (VM[posVM] == 0) { // fonction de base
                    processeur[VM[posVM + 1]]();
                } else { // fonction LAC
                    // pushStack(posVM, &retourne);
                    executeur(posVM);
                    // posVM = popStack(&retourne);
                }
            } else {
                // function is not found, judgement of type of lexemes
                //int data = type = 0;
                // assume the lexeme is an integer
                pushStack(atoi(lexemes[posLex].lex), &data);
                pushStack(ENTIER, &type);
            }
        }
        posLex++;
    } while (posLex <= t_c);
}

void compilateur() {
    int paraInCount = paraOutCount = 0; // stand for number of parameters of input, output and contained by the function orderly
    int posNFVM = finIndVM++, posNFLAC = ++finIndLAC;
    VM[finIndVM] = 1;
    int nameLength = LAC[finIndLAC] = strlen(lexemes[posLex++].lex);
    for (int i = 0; i < nameLength; i++) {
            LAC[++finIndLAC] = lexemes[posLex++].lex[i];
        }
    while (lexemes[posLex].lex != ';' && posLex <= t_c) {
        if (lexemes[posLex].type == C) {
            paraOutCount += 1;
            VM[++finIndVM] = posFonLITVM;
            // stock the chain.
            // storage();
            //VM[++finIndVM] = posChain;
        } else {
            int posLAC;
            if ((posLAC = isFunction(posLex, lexemes)) > 0) {
                // the lexemes is a function, mock the calculation process
                // check the availability of data/type stacks
                int lenName = LAC[posLAC];
                int paraIn = LAC[posLAC + lenName + 1];
                int paraOut = LAC[posLAC + lenName + paraIn + 2];
                int posVM = LAC[posLAC + lenName + paraIn + paraOut + 3];
                VM[++finIndVM] = posVM;
                paraInCount += paraIn - paraOutCount;
                paraOutCount = (paraOutCount - paraIn > 0) ? (paraOutCount - paraIn) : 0 + paraOut;
            } else {
                // function is not found,
                // assume the lexeme is an integer
                VM[++finIndVM] = posFonLITVM;
                VM[++finIndVM] = atoi(lexemes[posLex].lex);
            }
        }
        posLex++;
    };
    if (lexemes[posLex].lex == ';') {
        LAC[++finIndLAC] = paraInCount;
        for (int i = 0; i < paraInCount; i++) {
            LAC[++finIndLAC]] = ENTIER; // suppose
        }
        LAC[++finIndLAC] = paraOutCount;
        for (int i = 0; i < paraOutCount; i++) {
            LAC[++finIndLAC] = ENTIER;
        }
        LAC[++finIndLAC] = posNFVM; // VM position;
        LAC[++finIndLAC] = posNFLAC;

        //appending to VM
        VM[++finIndVM] = posFonFINVM;
        return;
    } else {
        exit(303);
    }
}

void executeur (int posVM) {
    if (VM[posVM] == 1) {
        while (VM[posVM] != posFonFINVM) {
            pushStack(posVM, &retourne);
            executeur(VM[posVM]);
            posVM = popStack(&retourne) + 1;
        }
    } else {
        processeur[VM[posVM + 1]];
    }
}