//
//  BNF_C.h
//  Compiler Lac
//
//  Created by Nicolas YING on 01/10/2016.
//  Copyright © 2016 Nicolas YING. All rights reserved.
//

#ifndef BNF_C_h
#define BNF_C_h

#include <stdio.h> // Fonctions
#include <stdlib.h> // Fonction exit
#include <string.h> // Fonction strncpy

// Définition des types de lexème
typedef enum lexeme_type {
    NOMBRE,
    PAREN_OU,
    PARENT_F,
    OP_LP,
    OP_HP,
} lexeme_type;

typedef struct lexeme {
    lexeme_type type;
    char valeur[20];
} lexeme;

typedef struct l_lexeme {
    lexeme tab[50];
    int N;
} l_lexeme;

typedef struct noeud * arbre;

struct noeud {
    lexeme * data;
    arbre leftChild;
    arbre rightChild;
};

struct returnFac {
    arbre oriFac;
    int indSpe;
};

// BNF fonctions
arbre somme (int d, int f);
arbre naturel (int d);
struct returnFac facteur (int d, int f);

int findBalancingParen (int f);
void add_divers(int type);
void add_nombre(void);
arbre assemble (lexeme * lex, arbre gau, arbre dro);
void traverse (arbre root);

const struct noeud ARBRE_ZERO = { &((struct lexeme){NOMBRE, "0\0"}), NULL, NULL};

int calculateString (char * string);

#endif /* BNF_C_h */
