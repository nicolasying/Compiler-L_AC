//
//  BNF_C.c
//  Compiler Lac
//
//  Created by Nicolas YING on 01/10/2016.
//  Copyright © 2016 Nicolas YING. All rights reserved.
//

#include "BNF_C.h"

arbre assemble (lexeme * lex, arbre gau, arbre dro) {
    arbre new = malloc(sizeof(struct noeud));
    new->data = lex;
    new->leftChild = gau;
    new->rightChild = dro;
    return new;
}

void traverse (arbre root) {
    
    if (root->leftChild != NULL) {
        traverse(root->leftChild);
        traverse(root->rightChild);
    }
    printf("%s ", root->data->valeur);
}

int calculateArbre (arbre root) {
     if (root->leftChild != NULL) {
        char ope = root->data->valeur[0];
        int resLeft = calculateArbre(root->leftChild);
        int resRight = calculateArbre(root->rightChild);
        switch (ope)
        {
        case '+': 
            return resLeft + resRight;
        case '-':
            return resLeft - resRight; 
        case 'x': 
            return resLeft * resRight; 
        case '/':
            if (resRight == 0) {
                printf("Division on 0! Result invalid.\n");
                return 0;
            }
            return resLeft / resRight; 
        default:
            printf("Unknown operation. Result invalid.\n");
            return 0;
        }
    } else {
        return atoi(root->data->valeur);
    }
}