//
//  Interprete
//  Compiler Lac
//
//  Created by Nicolas YING on 10/10/2016.
//  Copyright © 2016 Nicolas YING. All rights reserved.
//

#include "processeur.c"
#include "interpreter.h"
#include "interpreter_pre.c"
#include <string.h>
#include <stdlib.h>

extern lexeme_t lexemes[];
extern int t_c;
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

void displayStack (const pileBase ** pileData, const pileBase ** pileType) {
    printf("Stack : (top) ");
    pileBase * tempPointerD = * pileData, *tempPointerT = * pileType;
    while(tempPointerD->precedent != NULL) {
        if(tempPointerT->data == ENTIER) printf("%d ", tempPointerD->data);
        else if(tempPointerT->data == BOOLEAN) printf("%d ", tempPointerD->data);
        else if(tempPointerT->data == FLOATANT) printf("%f ", tempPointerD->data); // to be corrected.
        else if(tempPointerT->data == CHAINECHAR) printf("%s ", tempPointerD->data); // to be corrected.
    } 

}

pileBase *data, *type, *retourne;
int stringMem[200] = {0};
int stringPtr = 0;
int posLex;



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
    int length = lexlen(lexeme[i].lex);
    int position = LAC[finIndLAC];
    while (position > 0) {
        if (LAC[position] != length) {
            position = LAC[position - 1];
        } else {
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
    }
    return position;
}

void interpreteur() { //t_c est le nombre de lexemes
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
    // check for redefinition of functions
    int flag;
    if ((flag = isFunction(posLex + 1, lexemes)) && flag > 0) {
        printf("Redefinition of function.\n");
        exit(310);
    }

    int paraInCount = 0, paraOutCount = 0; // stand for number of parameters of input, output and contained by the function orderly
    int finIndVMl = finIndVM, finIndLACl = finIndLAC;
    int posNFVM = ++finIndVMl, posNFLAC = ++finIndLACl;
    VM[finIndVMl] = 1;
    int nameLength = LAC[finIndLACl] = lexlen((lexemes[++posLex].lex));
    for (int i = 0; i < nameLength; i++) {
            LAC[++finIndLACl] = lexemes[posLex].lex[i];
    }
    posLex++;
    while (posLex <= t_c && *(lexemes[posLex].lex) != ';' || lexlen(lexemes[posLex].lex) != 1 ) {
        if (lexemes[posLex].type == C) {
            paraOutCount += 1;
            VM[++finIndVMl] = posFonLITVM;
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
                VM[++finIndVMl] = posVM;
                paraInCount += paraIn - paraOutCount;
                paraOutCount = (paraOutCount - paraIn > 0) ? (paraOutCount - paraIn) : 0 + paraOut;
            } else {
                // function is not found,
                // assume the lexeme is an integer
                paraOutCount++;
                VM[++finIndVMl] = posFonLITVM;
                VM[++finIndVMl] = atoi(lexemes[posLex].lex);
            }
        }
        posLex++;
    };
    if (*(lexemes[posLex].lex) == ';' && lexlen(lexemes[posLex].lex) == 1) {
        LAC[++finIndLACl] = paraInCount;
        for (int i = 0; i < paraInCount; i++) {
            LAC[++finIndLACl] = ENTIER; // suppose
        }
        LAC[++finIndLACl] = paraOutCount;
        for (int i = 0; i < paraOutCount; i++) {
            LAC[++finIndLACl] = ENTIER;
        }
        LAC[++finIndLACl] = posNFVM; // VM position;
        LAC[++finIndLACl] = posNFLAC;

        //appending to VM
        VM[++finIndVMl] = posFonFINVM;
        finIndLAC = finIndLACl; 
        finIndVM = finIndVMl; // synchroniser
        return;
    } else {
        exit(303);
    }
}

void executeur (int posVM) {
    if (VM[posVM++] != 0) {
        //pushStack(posVM, &retourne);
        while (VM[posVM] != posFonFINVM) {
            pushStack(posVM, &retourne);
            executeur(VM[posVM]);
            posVM = popStack(&retourne) + 1;
        }
        //processeur[VM[posFonFINVM]]();
    } else {
        processeur[VM[posVM]]();
    }
}

int lexlen(char * lex) {
    int i = 0;
    while (lex[i] != '\0') {
        i++;
    }
    return i;
}