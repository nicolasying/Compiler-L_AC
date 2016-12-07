//
//  calculate.c
//  Compiler Lac
//
//  Grande contribution faite par Alain Chilles.
//  Created by Nicolas YING on 01/10/2016.
//  Copyright © 2016 Nicolas YING. All rights reserved.
//

#include "BNF_C.h"
#include "../common_component.h"
#include <string.h>

l_lexeme resultat;
int i = 0, j = 0;
lexeme lu;
static char * s;

extern struct noeud ARBRE_ZERO;

int calculateString (char * string) {
    
    #ifdef DEBUG
    printf("calculateString: \n");
    printString(string, 0);
    #endif // DEBUG

    // Transform the LAC string into C string, to reuse the ancient code written a while ago.
    int length = string[0];
    char stemp[100] = {'\0'};
    strncpy(&stemp[0], &string[1], length);
    s = &stemp[0];

    // lexicale
    resultat.N = 0;
    ARBRE_ZERO = (struct noeud) { &((struct lexeme){NOMBRE, "0\0"}), NULL, NULL};
    do {
        switch (s[i]) {
            case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                j++;
                break;
                
            case '+': case '-':
                add_divers(OP_LP);
                break;
                
            case 'x': case '/':
                add_divers(OP_HP);
                break;
                
            case '(':
                add_divers(PAREN_OU);
                break;
                
            case ')':
                add_divers(PARENT_F);
                break;
                
//            case ' ': case '\t':
//                j++;
//                break;
                
            default:
                printf("Error 9001! Unrecognized word: %c\n", s[i]);
                return -9001;
        }
    } while (s[++i] != 0);
    
    add_nombre();
    
    arbre arbreSyntax = somme(0, resultat.N-1);
    
    traverse(arbreSyntax); // debug
    printf("\n");
//    printf("\nL'expression %s est syntaxiquement %s\n", s, somme(0, resultat.N - 1) ? "correcte" : "incorrecte");
    
    return calculateArbre(arbreSyntax);
}

void add_nombre(void) {

    if (j != 0) {
        lu.type = NOMBRE;
        strncpy(lu.valeur, s + i - j, j);
        lu.valeur[j] = 0;  // une chaîne se termine par un 0.
        resultat.tab[resultat.N] = lu;
        #ifdef DEBUG
        printf("add_nombre: %s\n", lu.valeur);
        #endif // DEBUG

        resultat.N += 1;
    }
}

void add_divers(int type) {
    
    add_nombre();
    j = 0;
    lu.type = type;
    strncpy(lu.valeur, s + i, 1);
    lu.valeur[1] = 0;
    resultat.tab[resultat.N] = lu;
    #ifdef DEBUG
    printf("add_diver: %s, %d\n", lu.valeur, type);
    #endif // DEBUG
    resultat.N += 1;
}

arbre naturel (int d) {

//    printf("Calling naturel, d = %d\n", d);
    if (resultat.tab[d].type != NOMBRE) {
        printf("Calculate: Error 9103.\n");
        return NULL;
    } else {
//        printf("node: %s, left: null,right: null\n", resultat.tab[d].valeur);
        return assemble(&resultat.tab[d], NULL, NULL);
        // les nombres sont toujours des feuilles.
    }
}

int findBalancingParen (int f) {
    
//    printf("Calling balancing, f = %d\n", f);
    if (resultat.tab[f].type != PARENT_F) {
        printf("Calculate: Error 9101. not a closing parenthesis!\n");
        return (-9101);
    }
    
    int pile = 0; // pour balancer les parentheses
    
    while (f > 0 && pile >= 0) {
        switch (resultat.tab[--f].type) {
            case PARENT_F:
                pile++;
                break;
            case PAREN_OU:
                pile--;
                break;
            default:
                break;
        }
    }
    if (pile == -1) {
        return f;
    }
    printf("Calculate: Error 9102. didn't find a balanced opening parenthesis.\n");
    return(-9102);
    
}

struct returnFac facteur (int d, int f) {
//    printf("Calling facteur, d = %d, f = %d\n", d, f);
    struct returnFac errorStruct;
    int indSpe = f;
    // indSpe est une indication du bord de facteur, soit un parenthese, soit un +/-
    
    if (f == d && resultat.tab[f].type == NOMBRE) {
        indSpe = d-1;
//        printf("1 facteur returning indSpe = %d\n", d-1);
        return (struct returnFac){naturel(f), indSpe};
    }
    
    if (f < d) {
        printf("Calculate: Error 9106.\n");
        return(errorStruct);
    }
    
    if (resultat.tab[f].type == NOMBRE) {
        
        arbre right_child = naturel(f);
        
        if (resultat.tab[f-1].type == OP_HP) {
            
//            printf("1 facteur detected OP_HP\n");
            struct returnFac res = facteur(d, f-2);
//            printf("node: %s, left: %s,right: %s\n", resultat.tab[f-1].valeur, res.oriFac->data->valeur, right_child->data->valeur);
            return (struct returnFac){assemble(&resultat.tab[f-1], res.oriFac, right_child), res.indSpe};
            
        } else if (resultat.tab[f-1].type == OP_LP) {
            
            indSpe = f-1; // la partie facteur se finit, commencement d'assemblage d'arbre
//            printf("2 facteur returning indSpe = %d\n", f-1);
//            printf("node: %s, left: null,right: null\n", right_child->data->valeur);
            return (struct returnFac){right_child, indSpe};
        } else {
            printf("Calculate: Error 9105.\n");
            return(errorStruct);
        }
    } else if (resultat.tab[f].type == PARENT_F) {
        
        int opPar = findBalancingParen(f);
        arbre right_child = somme(opPar+1, f-1);
        
        if (resultat.tab[opPar-1].type == OP_HP) {

            //            printf("2 facteur detected OP_HP\n");
            struct returnFac res = facteur(d, opPar-2);
//            printf("node: %s, left: %s,right: %s\n", resultat.tab[opPar-1].valeur, res.oriFac->data->valeur, right_child->data->valeur);
            return (struct returnFac){assemble(&resultat.tab[opPar-1], res.oriFac, right_child), res.indSpe};

        } else if (resultat.tab[opPar-1].type == OP_LP) {
      
            indSpe = opPar-1;
//            printf("3 facteur returning indSpe = %d\n", opPar-1);
//            printf("node: %s, left: null,right: null\n", right_child->data->valeur);
            return (struct returnFac){right_child, indSpe};
        
        } else if (opPar == 0){
//            printf("node: %s, end of parenthesis\n", right_child->data->valeur);
//            if (right_child->leftChild != NULL) printf("left: %s\n",right_child->leftChild->data->valeur);
//            if (right_child->rightChild != NULL) printf("right: %s\n",right_child->rightChild->data->valeur);
            return (struct returnFac){right_child, opPar};
            
        } else {
            printf("Calculate: Error 9107. 107 exiting with opPar-1 = %d\n", opPar - 1);
            return (errorStruct);
        }
    } else {
        printf("Calculate: Error 9104.\n");
        return(errorStruct);
    }
}

arbre somme (int d, int f) {
//    printf("Calling somme, d = %d, f = %d\n", d, f);
    
    struct returnFac resFac = facteur(d, f);
    int intSpe = resFac.indSpe;
    arbre rightChild = resFac.oriFac;
    
    if (intSpe < d || (intSpe == d && resultat.tab[intSpe].type != OP_LP)) {
        // si l'expression est toute traitée
        return rightChild;
    
    } else if (intSpe == d && resultat.tab[intSpe].type == OP_LP) {
        // si il y a un +/- unair devant la partie d'expression
//        printf("1 d = %d\n", d);
//        printf("node: %s, left: %s,right: %s\n", resultat.tab[intSpe].valeur, "0", rightChild->data->valeur);
        return assemble(&resultat.tab[intSpe], &ARBRE_ZERO, rightChild);
        
    } else {
        // le cas ordinaire
//        printf("2 d = %d\n", d);
//        printf("node: %s, left: %s,right: %s\n", resultat.tab[intSpe].valeur, "somme", rightChild->data->valeur);
        return assemble(&resultat.tab[intSpe], somme(d, intSpe-1), rightChild);
    }
}
