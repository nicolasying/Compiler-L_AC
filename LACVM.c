//
//  LAC et VM definition
//  Compiler Lac
//
//  Proposer un table de syntax structuré
//  
//  Created by Nicolas YING on 10/12/2016.
//  Copyright © 2016 Nicolas YING. All rights reserved.
//

// definition temporaire de la table des symboles
typedef struct symbols {
    char * name;
    int paraIn;
    int * typeIn;
    int paraOut;
    int * typeOut;
} * symbols;

symbols LAC[100];
            // [0, 1, '+', 2, ENTIER, ENTIER, 1, ENTIER, 0, 1, 
            //     4, 's', 'w', 'a', 'p', 2, ENTIER, ENTIER, 2, ENTIER, ENTIER, 2, 10, 
            //     1, '.', 1, ENTIER, 0, 4, 22, 
            //     5, '(', 'l', 'i', 't', ')', 0, 0, 6, 29, 
            //     5, '(', 'f', 'i', 'n', ')', 0, 0, 8, 39, 
            //      ];
int VM[200] = [0, 0, 0, 1, 0, 2, 0, 3, 0, 4];

void addFunctionBase (const int finIndVM, cosnt int indProcesseur, const int finIndLAC, char * name, int paraIn, int typeIn[], int paraOut, int typeOut[]) {
    // appending to LAC
    // LAC[finIndLAC + 1] = LAC[LAC[finIndLAC]] + 1; // sequence number of the function in LAC
    LAC[finIndLAC] = malloc(sizeof(struct symbols));
    
    strncpy(LAC[finIndLAC + 2], name, LAC[finIndLAC + 1]);
    LAC[finIndLAC + 2 + LAC[finIndLAC + 1]] = paraIn;
    strncpy(LAC[finIndLAC + 3 + LAC[finIndLAC + 1]], typeIn, paraIn);
    LAC[finIndLAC + 3 + LAC[finIndLAC + 1] + paraIn] = paraOut;
    strncpy(LAC[finIndLAC + 4 + LAC[finIndLAC + 1] + paraIn], typeOut, paraOut);
    LAC[finIndLAC + 4 + LAC[finIndLAC + 1] + paraIn + paraOut] = finIndVM + 1// VM position;
    LAC[finIndLAC + 5 + LAC[finIndLAC + 1] + paraIn + paraOut] = finIndLAC + 1;

    //appending to VM
    VM[finIndVM + 1] = 0;
    VM[finIndVM + 2] = indProcesseur;
}

void (*foncBase)(void) processeur[] = [, ]

void initiateLACVM (void ){
    addFunctionBase();
}