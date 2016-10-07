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

