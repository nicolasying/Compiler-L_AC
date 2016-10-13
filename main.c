//
//  main.c
//  Compiler Lac
//
//  Created by Nicolas YING on 19/09/2016.
//  Copyright © 2016 Nicolas YING. All rights reserved.
//

#define MAX_FILE_LENGTH 1000
#define MAX_LEMEXE_NUMBER 100
#include "analex.h"
#include "analex.c"
#include "interpreter.c"
#include <stdio.h>
#include <stdlib.h>
lexeme_t lexemes[100];
int t_c;

int main(int argc, char * argv[]) {
    
//    if (argc != 2) {
//        return 3;
//    }
    
    FILE *fp;
    
    long size_file;
    
    //
    //fp = fopen("/Users/soshy/Documents/TPLA/factorielle.txt", "r");
    
    // get file size
    fp = fopen(argv[1], "r");
    fseek(fp, 0L, SEEK_END);
    size_file = ftell(fp);
    rewind(fp);

    //fgets(texte, MAX_FILE_LENGTH, fp);
    //fscanf(fp, "%[^\0]s", texte);
    //fread(texte, 1, size_file, fp); // root cause for SIGABRT
    
    // t-zhg solution
    // get file chars into one single string
    char texte[size_file];
    memset(texte, 0, size_file);
    char tmp[100];
    memset(tmp, 0, 100);
    
    while (fgets(tmp, 100, fp)) {
        strcat(texte, tmp);
    }
    
    fclose(fp);
    //printf("---main---\nRead Buffer: \n%s\n", texte);
    
    // pass to analyse lexicale
    t_c = analex(texte, lexemes);
    printf("---main---\nFin d'Analyse Lexicale!\n%d lexemes detected and stored in array.\n", t_c--);
    printf("---main---\nPas d'Analyse Syntaxique.\n");
    
    // pass to inteprete and compile

    initLACVMPro();
    interpreteur();
    printf("---main---\nFin d'interpreter.\n");

    // freeing malloc allocated memory
    for (; t_c >= 0; t_c--) {
        //printf("---main---\nfreeing:\n%s\n",lexemes[t_c].lex);
        free(lexemes[t_c].lex);
    }

    printf("---main---\nFin d'Analyse pour le moment.\n");
    
    return 0;
}
